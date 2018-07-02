//==============================================================================
//	
//	Copyright (c) 2002-
//	Authors:
//	* Dave Parker <david.parker@comlab.ox.ac.uk> (University of Oxford, formerly University of Birmingham)
//	
//------------------------------------------------------------------------------
//	
//	This file is part of PRISM.
//	
//	PRISM is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//	
//	PRISM is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	
//	You should have received a copy of the GNU General Public License
//	along with PRISM; if not, write to the Free Software Foundation,
//	Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//	
//==============================================================================

// includes
#include "PrismSparse.h"
#include <cmath>
#include <util.h>
#include <cudd.h>
#include <dd.h>
#include <odd.h>
#include <dv.h>
#include "sparse.h"
#include "PrismSparseGlob.h"
#include "jnipointer.h"
#include "prism.h"
#include "Measures.h"
#include "ExportIterations.h"
#include <memory>
#include <new>
#include <queue>
using namespace std;
#define alpha .05
//------------------------------------------------------------------------------
struct stpq
{
	int state;
	double delta;
};
// solve the linear equation system Ax=b with Gauss-Seidel/SOR

JNIEXPORT jlong __jlongpointer JNICALL Java_sparse_PrismSparse_PS_1PRIORITYQUEUESOR
(
JNIEnv *env,
jclass cls,
jlong __jlongpointer _odd,	// odd
jlong __jlongpointer rv,	// row vars
jint num_rvars,
jlong __jlongpointer cv,	// col vars
jint num_cvars,
jlong __jlongpointer _a,	// matrix A
jlong __jlongpointer _b,	// vector b (if null, assume all zero)
jlong __jlongpointer _init,	// init soln
jboolean transpose,	// transpose A? (i.e. solve xA=b not Ax=b?)
jboolean row_sums,	// use row sums for diags instead? (strictly speaking: negative sum of non-diagonal row elements)
jdouble omega,		// omega (over-relaxation parameter)
jboolean forwards	// forwards or backwards?
)
{
	// cast function parameters
	ODDNode *odd = jlong_to_ODDNode(_odd);		// odd
	DdNode **rvars = jlong_to_DdNode_array(rv); 	// row vars
	DdNode **cvars = jlong_to_DdNode_array(cv); 	// col vars
	DdNode *a = jlong_to_DdNode(_a);		// matrix A
	DdNode *b = jlong_to_DdNode(_b);		// vector b
	DdNode *init = jlong_to_DdNode(_init);		// init soln
	// mtbdds
	DdNode *reach = NULL, *diags = NULL, *id = NULL;
	// model stats
	int n;
	long nnz;
	// flags
	bool compact_a, compact_d, compact_b;
	// sparse matrix
	RMSparseMatrix *rmsm = NULL;
	CMSRSparseMatrix *cmsrsm = NULL;
	// vectors
	double *diags_vec = NULL, *b_vec = NULL, *soln = NULL;
	DistVector *diags_dist = NULL, *b_dist = NULL;
	// timing stuff
	long start1, start2, start3, stop;
	double time_taken, time_for_setup, time_for_iters;
	// misc
	int i, j, fb, l, h, iters, top, m1, m2, l1, h1;
	double d, x, sup_norm, kb, kbt, total_mults, self;
	bool done;
	int num_of_trans;	
	double* in_probs;		

	// exception handling around whole function
	try {
	
	// start clocks
	start1 = start2 = util_cpu_time();
	
	// get number of states
	n = odd->eoff + odd->toff;
	
	// get reachable states
	reach = odd->dd;
	
	// make local copy of a
	Cudd_Ref(a);
	
	// remove and keep diagonal entries of matrix A
	id =  DD_Identity(ddman, rvars, cvars, num_rvars);
	Cudd_Ref(reach);
	id = DD_And(ddman, id, reach);
	Cudd_Ref(id);
	Cudd_Ref(a);
	diags = DD_Apply(ddman, APPLY_TIMES, id, a);
	Cudd_Ref(id);
	a = DD_ITE(ddman, id, DD_Constant(ddman, 0), a);
	
	// build sparse matrix
	PS_PrintToMainLog(env, "\nBuilding sparse matrix... ");
	// if requested, try and build a "compact" version
	compact_a = true;
	cmsrsm = NULL;
	if (compact) cmsrsm = build_cmsr_sparse_matrix(ddman, a, rvars, cvars, num_rvars, odd, transpose);
	if (cmsrsm != NULL) {
		nnz = cmsrsm->nnz;
		kb = cmsrsm->mem;
	}
	// if not or if it wasn't possible, built a normal one
	else {
		compact_a = false;
		rmsm = build_rm_sparse_matrix(ddman, a, rvars, cvars, num_rvars, odd, transpose);
		nnz = rmsm->nnz;
		kb = rmsm->mem;
	}
	kbt = kb;
	// print some info
	PS_PrintToMainLog(env, "[n=%d, nnz=%d%s] ", n, nnz, compact_a?", compact":"");
	PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	
	// get vector of diags, either by extracting from mtbdd or
	// by doing (negative, non-diagonal) row sums of original A matrix
	PS_PrintToMainLog(env, "Creating vector for diagonals... ");
	if (!row_sums) {
		diags = DD_MaxAbstract(ddman, diags, cvars, num_cvars);
		diags_vec = mtbdd_to_double_vector(ddman, diags, rvars, num_rvars, odd);
	} else {
		diags_vec = compact_a ? cmsr_negative_row_sums(cmsrsm, transpose) : rm_negative_row_sums(rmsm, transpose);
	}
	// if any of the diagonals are zero, set them to one - avoids division by zero errors later
	// strictly speaking, such matrices shouldn't work for this iterative method
	// but they do occur, e.g. for steady-state computation of a bscc, this fixes it
	for (i = 0; i < n; i++) diags_vec[i] = (diags_vec[i] == 0) ? 1.0 : diags_vec[i];
	// try and convert to compact form if required
	compact_d = false;
	if (compact) {
		if ((diags_dist = double_vector_to_dist(diags_vec, n))) {
			compact_d = true;
			delete diags_vec; diags_vec = NULL;
		}
	}
	kb = (!compact_d) ? n*8.0/1024.0 : (diags_dist->num_dist*8.0+n*2.0)/1024.0;
	kbt += kb;
	if (compact_d) PS_PrintToMainLog(env, "[dist=%d, compact] ", diags_dist->num_dist);
	PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	
	// invert diagonal
	if (!compact_d) {
		for (i = 0; i < n; i++) diags_vec[i] = 1.0 / diags_vec[i];
	} else {
		for (i = 0; i < diags_dist->num_dist; i++) diags_dist->dist[i] = 1.0 / diags_dist->dist[i];
	}
	
	num_of_trans = diags_dist->num_dist;		

	// build b vector (if present)
	if (b != NULL) {
		PS_PrintToMainLog(env, "Creating vector for RHS... ");
		b_vec = mtbdd_to_double_vector(ddman, b, rvars, num_rvars, odd);
		// try and convert to compact form if required
		compact_b = false;
		if (compact) {
			if ((b_dist = double_vector_to_dist(b_vec, n))) {
				compact_b = true;
				delete b_vec; b_vec = NULL;
			}
		}
		kb = (!compact_b) ? n*8.0/1024.0 : (b_dist->num_dist*8.0+n*2.0)/1024.0;
		kbt += kb;
		if (compact_b) PS_PrintToMainLog(env, "[dist=%d, compact] ", b_dist->num_dist);
		PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	}
	// create solution/iteration vectors
	PS_PrintToMainLog(env, "Allocating iteration vector... ");
	soln = mtbdd_to_double_vector(ddman, init, rvars, num_rvars, odd);
	kb = n*8.0/1024.0;
	kbt += kb;
	PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	
	// print total memory usage
	PS_PrintMemoryToMainLog(env, "TOTAL: [", kbt, "]\n");
	
	// get setup time
	stop = util_cpu_time();
	time_for_setup = (double)(stop - start2)/1000;
	start2 = stop;
	start3 = stop;
	
	// start iterations
	iters = 0;
	done = false;
	PS_PrintToMainLog(env, "\nStarting iterations...\n");
		
	int* dns_start;					
	double *non_zeros;
	unsigned char *row_counts;
	int *row_starts;
	bool use_counts;
	unsigned int *cols;
	double *dist;
	int dist_shift;
	int dist_mask;
	if (!compact_a) {
		non_zeros = rmsm->non_zeros;
		row_counts = rmsm->row_counts;
		row_starts = (int *)rmsm->row_counts;
		use_counts = rmsm->use_counts;
		cols = rmsm->cols;
	} else {
		row_counts = cmsrsm->row_counts;
		row_starts = (int *)cmsrsm->row_counts;
		use_counts = cmsrsm->use_counts;
		cols = cmsrsm->cols;
		dist = cmsrsm->dist;
		dist_shift = cmsrsm->dist_shift;
		dist_mask = cmsrsm->dist_mask;
	}

	bool *selected = new bool[n];

	int *pre_start = new int[n];
	int *pre_end = new int[n];
	int *pre_freq = new int[n];

 	int *pre_state = new int[nnz + 500];
	int *state_order = new int[n];

	for(i = 0; i < n; i++)
	{
		pre_freq[i] = 0;
		selected[i] = false;
	}

	for(i = 0; i < n; i++)
		for(j = row_starts[i]; j < row_starts[i+1]; j++)
			if(fabs(non_zeros[j]) >= alpha / (row_starts[i+1] - row_starts[i]))
				pre_freq[cols[j]]++;

	pre_start[0] = 0;
	pre_end[0] = pre_freq[0];
	for(i = 1; i < n; i++)
	{	
		pre_start[i] = pre_end[i-1];
		pre_end[i] = pre_end[i-1] + pre_freq[i];
	}	

	int top = 0, _a, small, good_states, tot_mults, good_mults, m;
	double effic1, effic2, change_sum;
	bool flag = true;

	unsigned int int_temp;						

	flag = true;
	total_mults = 0;
	for(i = 0; i < n; i++)
		pre_freq[i] = pre_start[i];

	for(i = 0; i < n; i++){
		for(j = row_starts[i]; j < row_starts[i+1]; j++)
			if(fabs(non_zeros[j]) >= alpha / (row_starts[i+1] - row_starts[i]))
				pre_state[pre_freq[cols[j]]++] = i;
	}


	l = 0;
	stpq st1;

	auto cmp = [](stpq left, stpq right){return (left.delta < right.delta);};
	std::priority_queue<stpq, std::vector<stpq>, decltype(cmp)> pq(cmp);

	iters++;		
	sup_norm = 0.0;
	change_sum = 0;
	// store local copies of stuff
	// matrix multiply
	l = nnz; h = 0;
	for (fb = 0; fb < n; fb++) 
	{
		
		// loop actually over i
		// (can do forwards or backwards sor/gs)
		i = fb;
		d = (b == NULL) ? 0.0 : ((!compact_b) ? b_vec[i] : b_dist->dist[b_dist->ptrs[i]]);

		l = row_starts[i]; 
		h = row_starts[i+1];
		
		// "row major" version

		if (!compact_a) {

			//if(flag){
			for (j = l; j < h; j++) 
				d -= non_zeros[j] * soln[cols[j]];
			//}
		// "compact msr" version
		} 
		// divide by diagonal (multiply by inverted diagonal)
		if (!compact_d) d *= diags_vec[i]; else d *= diags_dist->dist[diags_dist->ptrs[i]];
		// over-relaxation
		if (omega != 1.0) {
			d = ((1-omega) * soln[i]) + (omega * d);
		}

		// compute norm for convergence
		// (note we must do this inside the loop because we only store one vector for sor/gauss-seidel)
		x = fabs(d - soln[i]);
		if (term_crit == TERM_CRIT_RELATIVE && d > 0) {
			x /= d;
		}
		if(x > term_crit_param)
		{
			x = fabs(d - soln[i]);
			st1.state = i;
			st1.delta = x;
			pq.push(st1);
			selected[i] = true;
		}	
		change_sum += x;
		if (x > sup_norm) sup_norm = x;
		// set vector element
		soln[i] = d;
	}		

	if (sup_norm < term_crit_param) 
		done = true;				

		iters = 0;	
		while(iters < 1e+5)
		{
			iters++;
			st1 = pq.top();
			pq.pop(); 
			printf("\n %f state=%d, iters = %d, pq size = %d ", st1.delta, st1, iters, pq.size());
			selected[st1.state] = false;
			for(m = pre_start[st1.state]; m < pre_end[st1.state]; m++)
			{
				i = pre_state[m];
				d = (b == NULL) ? 0.0 : ((!compact_b) ? b_vec[i] : b_dist->dist[b_dist->ptrs[i]]);

				l = row_starts[i]; 
				h = row_starts[i+1];
				
				// "row major" version

				if (!compact_a) {

					//if(flag){
					for (j = l; j < h; j++) 
						d -= non_zeros[j] * soln[cols[j]];
					//}
				// "compact msr" version
				} 
				// divide by diagonal (multiply by inverted diagonal)
				if (!compact_d) d *= diags_vec[i]; else d *= diags_dist->dist[diags_dist->ptrs[i]];
				// over-relaxation
				if (omega != 1.0) {
					d = ((1-omega) * soln[i]) + (omega * d);
				}

				// compute norm for convergence
				// (note we must do this inside the loop because we only store one vector for sor/gauss-seidel)
				x = fabs(d - soln[i]);
				if (term_crit == TERM_CRIT_RELATIVE && d > 0) {
					x /= d;
				}
				if(x > term_crit_param && selected[i] == false)
				{
					x = fabs(d - soln[i]);
					st1.state = i;
					st1.delta = x;
					pq.push(st1);
					selected[i] = true;
				}	
				if (x > sup_norm) sup_norm = x;
				// set vector element
				soln[i] = d;
			}
			if(pq.size() <= 0)
				{done = true;break;}
		}


	// stop clocks
	stop = util_cpu_time();
	time_for_iters = (double)(stop - start2)/1000;
	time_taken = (double)(stop - start1)/1000;
	
	// print iters/timing info
	PS_PrintToMainLog(env, "\n%s%s: %d iterations in %.2f seconds (average %.6f, setup %.2f)\n", forwards?"":"Backwards ", (omega == 1.0)?"Gauss-Seidel":"SOR", iters, time_for_iters, time_for_iters/iters, time_for_setup);
	
	// if the iterative method didn't terminate, this is an error
	if (!done) { delete soln; soln = NULL; PS_SetErrorMessage("Iterative method did not converge within %d iterations.\nConsider using a different numerical method or increasing the maximum number of iterations", iters); }
	
	// catch exceptions: register error, free memory
	} catch (std::bad_alloc e) {
		PS_SetErrorMessage("Out of memory");
		if (soln) delete[] soln;
		soln = 0;
	}
	
	// free memory
	if (a) Cudd_RecursiveDeref(ddman, a);
	if (id) Cudd_RecursiveDeref(ddman, id);
	if (diags) Cudd_RecursiveDeref(ddman, diags);
	if (rmsm) delete rmsm;
	if (cmsrsm) delete cmsrsm;
	if (diags_vec) delete[] diags_vec;
	if (diags_dist) delete diags_dist;
	if (b_vec) delete[] b_vec;
	if (b_dist) delete b_dist;

	printf("\n\nNumber of scallar multiplications: %dM \n", (int) (total_mults / 1000000));
	return ptr_to_jlong(soln);
}

//------------------------------------------------------------------------------

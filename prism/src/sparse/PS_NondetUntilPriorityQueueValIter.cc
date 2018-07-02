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
#include <math.h>
#include <util.h>
#include <cudd.h>
#include <dd.h>
#include <odd.h>
#include <dv.h>
#include "sparse.h"
#include "prism.h"
#include "PrismNativeGlob.h"
#include "PrismSparseGlob.h"
#include "jnipointer.h"
#include <new>
#include <queue>
using namespace std;
//------------------------------------------------------------------------------
struct stpq
{
	int state;
	double delta;
};

JNIEXPORT jlong __jlongpointer JNICALL Java_sparse_PrismSparse_PS_1NondetUntilPriorityQueueValIter
(
JNIEnv *env,
jclass cls,
jlong __jlongpointer t,		// trans matrix
jlong __jlongpointer ta,	// trans action labels
jobject synchs,
jlong __jlongpointer od,	// odd
jlong __jlongpointer rv,	// row vars
jint num_rvars,
jlong __jlongpointer cv,	// col vars
jint num_cvars,
jlong __jlongpointer ndv,	// nondet vars
jint num_ndvars,
jlong __jlongpointer y,		// 'yes' states
jlong __jlongpointer m,		// 'maybe' states
jboolean min,				// min or max probabilities (true = min, false = max)
jlong _strat				// strategy storage
)
{
	// cast function parameters
	DdNode *trans = jlong_to_DdNode(t);				// trans matrix
	DdNode *trans_actions = jlong_to_DdNode(ta);	// trans action labels
	ODDNode *odd = jlong_to_ODDNode(od); 			// reachable states
	DdNode **rvars = jlong_to_DdNode_array(rv); 	// row vars
	DdNode **cvars = jlong_to_DdNode_array(cv); 	// col vars
	DdNode **ndvars = jlong_to_DdNode_array(ndv);	// nondet vars
	DdNode *yes = jlong_to_DdNode(y);				// 'yes' states
	DdNode *maybe = jlong_to_DdNode(m); 			// 'maybe' states
	int *strat = (int *)jlong_to_ptr(_strat);		// strategy storage

	// mtbdds
	DdNode *a = NULL, *tmp = NULL;
	// model stats
	int n, nc;
	long nnz;
	// sparse matrix
	NDSparseMatrix *ndsm = NULL;
	// vectors
	double *yes_vec = NULL, *soln = NULL, /**soln2 = NULL,*/ *tmpsoln = NULL;
	// timing stuff
	long start1, start2, start3, stop;
	double time_taken, time_for_setup, time_for_iters, self;
	// adversary stuff
	int export_adv_enabled = export_adv;
	bool adv_loop = false;
	FILE *fp_adv = NULL;
	int adv_j;
	bool adv_new;
	int *adv = NULL;
	// action info
	jstring *action_names_jstrings;
	const char** action_names = NULL;
	int num_actions;
	// misc
	int i, j, k, l1, h1, l2, h2, iters, m1, m2;
	double d1, d2, x, sup_norm, kb, kbt;
	bool done, first;
	// exception handling around whole function
	try {
	
	// start clocks	
	start1 = start2 = util_cpu_time();
	
	// get a - filter out rows
	Cudd_Ref(trans);
	Cudd_Ref(maybe);
	a = DD_Apply(ddman, APPLY_TIMES, trans, maybe);
	
	// When computing maximum reachability probabilities,
	// we can safely remove any probability 1 self-loops for efficiency.
	// This might leave some states with no choices (only if no precomp done)
	// but this is not a problem, for value iteration.
	// This is also motivated by the fact that this fixes some simple problem
	// cases for adversary generation.
	if (!min) {
		Cudd_Ref(a);
		tmp = DD_And(ddman, DD_Equals(ddman, a, 1.0), DD_Identity(ddman, rvars, cvars, num_rvars));
		a = DD_ITE(ddman, tmp, DD_Constant(ddman, 0), a);
	}
	
	// get number of states
	n = odd->eoff + odd->toff;
	// build sparse matrix
	PS_PrintToMainLog(env, "\nBuilding sparse matrix(FOR GS)... ");
	ndsm = build_nd_sparse_matrix(ddman, a, rvars, cvars, num_rvars, ndvars, num_ndvars, odd);
	// get number of transitions/choices
	nnz = ndsm->nnz;
	nc = ndsm->nc;
	kb = ndsm->mem;
	kbt = kb;
	// print out info
	PS_PrintToMainLog(env, "[n=%d, nc=%d, nnz=%d, k=%d] ", n, nc, nnz, ndsm->k);
	PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	
	// if needed, and if info is available, build a vector of action indices for the MDP
	if (export_adv_enabled != EXPORT_ADV_NONE || strat != NULL) {
		if (trans_actions != NULL) {
			PS_PrintToMainLog(env, "Building action information... ");
			// first need to filter out unwanted rows
			Cudd_Ref(trans_actions);
			Cudd_Ref(maybe);
			tmp = DD_Apply(ddman, APPLY_TIMES, trans_actions, maybe);
			// then convert to a vector of integer indices
			build_nd_action_vector(ddman, a, tmp, ndsm, rvars, cvars, num_rvars, ndvars, num_ndvars, odd);
			Cudd_RecursiveDeref(ddman, tmp);
			kb = n*4.0/1024.0;
			kbt += kb;
			PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
			// also extract list of action names from 'synchs'
			get_string_array_from_java(env, synchs, action_names_jstrings, action_names, num_actions);
		} else {
			PS_PrintWarningToMainLog(env, "Action labels are not available for adversary generation.", export_adv_filename);
		}
	}
	
	// get vector for yes
	PS_PrintToMainLog(env, "Creating vector for yes...");
	yes_vec = mtbdd_to_double_vector(ddman, yes, rvars, num_rvars, odd);
	kb = n*8.0/1024.0;
	kbt += kb;
	PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	
	// create solution/iteration vectors
	PS_PrintToMainLog(env, "Allocating iteration vectors... ");
	soln = new double[n];
//	soln2 = new double[n];		Canceled for Gauss-Seidel.
	kb = n*8.0/1024.0;
	kbt += 2*kb;
	PS_PrintMemoryToMainLog(env, "[2 x ", kb, "]\n");
	
	// if required, create storage for adversary and initialise
	if (export_adv_enabled != EXPORT_ADV_NONE || strat != NULL) {
		PS_PrintToMainLog(env, "Allocating adversary vector... ");
		// Use passed in (pre-filled) array, if provided
		if (strat) {
			adv = strat;
		} else {
			adv = new int[n];
			// Initialise all entries to -1 ("don't know")
			for (i = 0; i < n; i++) {
				adv[i] = -1;
			}
		}
		kb = n*sizeof(int)/1024.0;
		kbt += kb;
		PS_PrintMemoryToMainLog(env, "[", kb, "]\n");
	}
	
	// print total memory usage
	PS_PrintMemoryToMainLog(env, "TOTAL: [", kbt, "]\n");
	
	// initial solution is yes 
	for (i = 0; i < n; i++) {
		soln[i] = yes_vec[i];
		//if (soln[i]) printf("yes[%d] := %f;\n", i+1, yes[i]);
	}
	
	// get setup time
	stop = util_cpu_time();
	time_for_setup = (double)(stop - start2)/1000;
	start2 = stop;
	start3 = stop;
	
	// start iterations
	iters = 0;
	done = false;
	PS_PrintToMainLog(env, "\nStarting iterations...\n");
	
	// open file to store adversary (if required)
	if (export_adv_enabled != EXPORT_ADV_NONE) {
		fp_adv = fopen(export_adv_filename, "w");
		if (fp_adv) {
			fprintf(fp_adv, "%d ?\n", n);
		} else {
			PS_PrintWarningToMainLog(env, "Adversary generation cancelled (could not open file \"%s\").", export_adv_filename);
			export_adv_enabled = EXPORT_ADV_NONE;
		}
	}
	
	// store local copies of stuff
	double *non_zeros = ndsm->non_zeros;
	unsigned char *row_counts = ndsm->row_counts;
	unsigned char *choice_counts = ndsm->choice_counts;
	bool use_counts = ndsm->use_counts;
	unsigned int *cols = ndsm->cols;
	int* row_starts;
	int* choice_starts;

	if(use_counts)
	{
		row_starts = new int[n+1]; 
		//choice_starts;
		row_starts[0] = 0;
		for(i = 1; i <= n; i++)
			row_starts[i] = row_starts[i - 1] + row_counts[i-1]; 

		choice_starts = new int[row_starts[n]+1]; 
		choice_starts[0] = 0;
		for(i = 1; i <= row_starts[n]; i++)
			choice_starts[i] = choice_starts[i - 1] + choice_counts[i-1]; 
		
	}
	else
	{
		row_starts = (int *)ndsm->row_counts;
		choice_starts = (int *)ndsm->choice_counts;
	}


	int *pre_start = new int[n];
	int *pre_end = new int[n];
	int *pre_freq = new int[n];
 	int *pre_state = new int[choice_starts[row_starts[n]-1]+500];
	int *state_order = new int[n];
	bool* selected = new bool[n];
	int top, l;
	top = 0;

	for(i = 0; i < n; i++)
	{	
		selected[i] = false;
		pre_freq[i] = 0;
	}

	for(i = 0; i < n; i++)
		for(j = row_starts[i]; j < row_starts[i+1]; j++)
			for(k = choice_starts[j]; k < choice_starts[j+1]; k++)
				pre_freq[cols[k]]++;

	pre_start[0] = 0;
	pre_end[0] = pre_freq[0];
	for(i = 1; i < n; i++)
	{
		pre_start[i] = pre_end[i-1];
		pre_end[i] = pre_end[i-1] + pre_freq[i];
	}		

	for(i = 0; i < n; i++)
		pre_freq[i] = pre_start[i];

	for(i = 0; i < n; i++)
		for(j = row_starts[i]; j < row_starts[i+1]; j++)
			for(k = choice_starts[j]; k < choice_starts[j+1]; k++)
					pre_state[pre_freq[cols[k]]++] = i;
	stpq st1;

	auto cmp = [](stpq left, stpq right){return (left.delta < right.delta);};
	std::priority_queue<stpq, std::vector<stpq>, decltype(cmp)> pq(cmp);


//	while (!done && iters < max_iters) {
		
		iters++;
		sup_norm = 0.0;
		// do matrix multiplication and min/max
		h1 = h2 = 0;
		for (i = 0; i < n; i++) 
		{
			d1 = 0.0; // initial value doesn't matter		
			first = true; // (because we also remember 'first')
			adv_new = false;
			l1 = row_starts[i]; h1 = row_starts[i+1]; 
			if(h1 <= l1)
				continue;
			for (j = l1; j < h1; j++) 
			{
				self = 1;
				d2 = 0;
				l2 = choice_starts[j]; h2 = choice_starts[j+1]; 
				//for (k = l2; k < h2; k++) {
				//	d2 += non_zeros[k] * soln[cols[k]];
				//}
				for(k = l2; k < h2; k++)
					if(cols[k] != i)
						d2 += non_zeros[k] * soln[cols[k]];
					else
						self -= non_zeros[k];
				if(self > 0)d2 /= self;else d2 = soln[i];
				if (first || (min&&(d2<d1)) || (!min&&(d2>d1))) {
					d1 = d2;
					// if adversary generation is enabled, remember optimal choice
					if (export_adv_enabled != EXPORT_ADV_NONE || strat != NULL) {
						// for max, only remember strictly better choices
						// (this resolves problems with end components)
						if (!min) {
							if (adv[i] == -1 || (d1>soln[i])) {
								adv[i] = j;
							}
						}
						// for min, this is straightforward
						// (in fact, could do it at the end of value iteration, but we don't)
						else {
							adv[i] = j;
						}
					}
				}
				first = false;
			}
			// set vector element
			// (if no choices, use value of yes)
			x = (h1 > l1) ? fabs(d1 - soln[i]) : 0;
			if(x > 0)
			{
				selected[i] = true;
				st1.state = i;	
				st1.delta = x;
				pq.push(st1);
			}			
			soln[i] = (h1 > l1) ? d1 : yes_vec[i];
			if (term_crit == TERM_CRIT_RELATIVE) 
			{
				if(soln[i] > 0)
					x /= soln[i];
				else 
					if(x > 0)	
						x = 1;
					else x = 0;
			}
			if (x > sup_norm) 
				sup_norm = x;			
		}
		
		iters = 0;	
		while(iters < 1+9)
		{iters++;
			st1 = pq.top();
			pq.pop();
			selected[st1.state] = false;
			for(m = pre_start[st1.state]; m < pre_end[st1.state]; m++)
			{
				i = pre_state[m];
				d1 = 0.0; // initial value doesn't matter		
				first = true; // (because we also remember 'first')
				adv_new = false;
				l1 = row_starts[i]; h1 = row_starts[i+1]; 
				if(h1 <= l1)
					continue;
				for (j = l1; j < h1; j++) 
				{
					self = 1;
					d2 = 0;
					l2 = choice_starts[j]; h2 = choice_starts[j+1]; 
					//for (k = l2; k < h2; k++) {
					//	d2 += non_zeros[k] * soln[cols[k]];
					//}
					for(k = l2; k < h2; k++)
						if(cols[k] != i)
							d2 += non_zeros[k] * soln[cols[k]];
						else
							self -= non_zeros[k];
					if(self > 0)d2 /= self;else d2 = soln[i];
					if (first || (min&&(d2<d1)) || (!min&&(d2>d1))) {
						d1 = d2;
					}
					first = false;
				}
				// set vector element
				// (if no choices, use value of yes)
				x = (h1 > l1) ? fabs(d1 - soln[i]) : 0;
				if (term_crit == TERM_CRIT_RELATIVE) 
				{
					if(soln[i] > 0)
						x /= soln[i];
					else 
						if(x > 0)	
							x = 1;
						else x = 0;
				}
				if(x > term_crit_param && selected[i] == false)
				{
					x = (h1 > l1) ? fabs(d1 - soln[i]) : 0;
					selected[i] = true;
					st1.state = i;	
					st1.delta = x;
					//if(iters%9000 == 5990)printf("\n%f  %d %d", x, iters, pq.size());
					pq.push(st1);
				}			
				soln[i] = (h1 > l1) ? d1 : yes_vec[i];				
			}
			if(pq.size() <= 0)
				break;
		}

		done = true;
		
		
		// print occasional status update
		if ((util_cpu_time() - start3) > UPDATE_DELAY) {
			PS_PrintToMainLog(env, "Iteration %d: max %sdiff=%f", iters, (term_crit == TERM_CRIT_RELATIVE)?"relative ":"", sup_norm);
			PS_PrintToMainLog(env, ", %.2f sec so far\n", ((double)(util_cpu_time() - start2)/1000));
			start3 = util_cpu_time();
		}
		
		// prepare for next iteration
		tmpsoln = soln;
		//soln = soln2;
		//soln2 = tmpsoln;
		

	
	// Traverse matrix to extract adversary
	if (export_adv_enabled != EXPORT_ADV_NONE) {
		h1 = h2 = 0;
		for (i = 0; i < n; i++) {
			if (!use_counts) { l1 = row_starts[i]; h1 = row_starts[i+1]; }
			else { l1 = h1; h1 += row_counts[i]; }
			// Have to loop through all choices (to compute offsets)
			for (j = l1; j < h1; j++) {
				if (!use_counts) { l2 = choice_starts[j]; h2 = choice_starts[j+1]; }
				else { l2 = h2; h2 += choice_counts[j]; }
				// But only output a choice if it is in the adversary
				if (j == adv[i]) {
					for (k = l2; k < h2; k++) {
						switch (export_adv_enabled) {
						case EXPORT_ADV_DTMC:
							fprintf(fp_adv, "%d %d %g", i, cols[k], non_zeros[k]); break;
						case EXPORT_ADV_MDP:
							fprintf(fp_adv, "%d 0 %d %g", i, cols[k], non_zeros[k]); break;
						}
						if (ndsm->actions != NULL) fprintf(fp_adv, " %s", ndsm->actions[j]>0?action_names[ndsm->actions[j]-1]:"");
						fprintf(fp_adv, "\n");
					}
				}
			}
		}
	}
	
	// stop clocks
	stop = util_cpu_time();
	time_for_iters = (double)(stop - start2)/1000;
	time_taken = (double)(stop - start1)/1000;
	
	// print iterations/timing info
	PS_PrintToMainLog(env, "\nIterative method: %d iterations in %.2f seconds (average %.6f, setup %.2f)\n", iters, time_for_iters, time_for_iters/iters, time_for_setup);
	
	// if the iterative method didn't terminate, this is an error
	if (!done) { delete soln; soln = NULL; PS_SetErrorMessage("Iterative method did not converge within %d iterations.\nConsider using a different numerical method or increasing the maximum number of iterations", iters); }
	
	// close file to store adversary (if required)
	if (export_adv_enabled != EXPORT_ADV_NONE) {
		fclose(fp_adv);
		PS_PrintToMainLog(env, "\nAdversary written to file \"%s\".\n", export_adv_filename);
	}
		
	// convert strategy indices from choices to actions
	if (strat != NULL) {
		for (i = 0; i < n; i++) {
			if (adv[i] > 0) strat[i] = ndsm->actions[adv[i]] - 1;
		}
	}
	
	// catch exceptions: register error, free memory
	} catch (std::bad_alloc e) {
		PS_SetErrorMessage("Out of memory");
		if (soln) delete[] soln;
		soln = 0;
	}
	
	// free memory
//	if (a) Cudd_RecursiveDeref(ddman, a);
//	if (ndsm) delete ndsm;
	if (yes_vec) delete[] yes_vec;
//	if (soln2) delete[] soln2;
	if (strat == NULL && adv) delete[] adv;
	if (action_names != NULL) {
		release_string_array_from_java(env, action_names_jstrings, action_names, num_actions);
	}
	
	return ptr_to_jlong(soln);
}

//------------------------------------------------------------------------------


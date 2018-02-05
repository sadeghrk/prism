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

//------------------------------------------------------------------------------

JNIEXPORT jlong __jlongpointer JNICALL Java_sparse_PrismSparse_PS_1NondetUntilImprovedModPI2
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
	DdNode *trans_actions = jlong_to_DdNode(ta);	// trans action labels t
	ODDNode *odd = jlong_to_ODDNode(od); 			// reachable states
	DdNode **rvars = jlong_to_DdNode_array(rv); 	// row vars
	DdNode **cvars = jlong_to_DdNode_array(cv); 	// col vars
	DdNode **ndvars = jlong_to_DdNode_array(ndv);	// nondet vars
	DdNode *yes = jlong_to_DdNode(y);				// 'yes' states
	DdNode *maybe = jlong_to_DdNode(m); 			// 'maybe' states
	int *strat = (int *)jlong_to_ptr(_strat);		// strategy storage
	
	int numTransitions;
	int one_iter_mults;

	// mtbdds
	DdNode *a = NULL, *tmp = NULL;
	// model stats
	int n, nc;
	long nnz;
	// sparse matrix
	NDSparseMatrix *ndsm = NULL;
	// vectors
	double *yes_vec = NULL, *soln = NULL, *tmpsoln = NULL;
	// timing stuff
	long start1, start2, start3, stop;
	double time_taken, time_for_setup, time_for_iters , delta; 		
	// adversary stuff
	int export_adv_enabled = export_adv;
	bool adv_loop = false;
	FILE *fp_adv = NULL;
	int adv_j, small;
	bool terminate;
	int *adv = NULL;
	int adv_temp;
	// action info
	jstring *action_names_jstrings;
	const char** action_names = NULL;
	int num_actions;
	double threshold = .75;
	// misc
	int i, j, k, h, l, l1, h1, l2, h2, iters, localitr, left, int_temp, right, temp_left;
	double d1, d2, x, sup_norm, kb, kbt, d3, inp_sum;
	bool done, first, flag;
	double self; 
	double total_mults;
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
	PS_PrintToMainLog(env, "\nBuilding sparse matrix (FOR POLICY ITERASION)... ");
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
	tmpsoln = new double[n];		//Canceled for Gauss-Seidel.
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
		soln[i] = tmpsoln[i] = yes_vec[i];
		//if (soln[i]) printf("yes[%d] := %f;\n", i+1, yes[i]);
	}
	
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
	// store local copies of stuff
	double *non_zeros = ndsm->non_zeros;
	unsigned char *row_counts = ndsm->row_counts;
	bool use_counts = ndsm->use_counts;
	unsigned char *choice_counts = ndsm->choice_counts;
	unsigned int *cols = ndsm->cols;
	double x, dbl_temp;
	double *low_soln = new double[n];
	double *low_self = new double[n];
		
	int *row_starts; 
	int *adv_starts; 
	int *choice_starts;
	int dtmc_max_trans = 1000;
	int tmp;
	if(use_counts)
	{
		row_starts = new int[n+1];
		choice_starts;
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

	for(i = 0; i < n; i++)
	{
		tmp = 0;
		for(j = row_starts[i]; j < row_starts[i+1]; j++)
		 	if(choice_starts[j+1] - choice_starts[j] > tmp)
				tmp = choice_starts[j+1] - choice_starts[j];
		dtmc_max_trans += tmp;
	}

	int maybe_states = 0;
	one_iter_mults = 0;
	int* high_start = new int[n];

	adv_starts = new int[n+1]; 		
	for(i = 0; i < n; i++)	
	{
		if(row_starts[i] >= row_starts[i+1])
			continue;
		adv_starts[i] = row_starts[i];
		if(row_starts[i + 1] > row_starts[i])
			maybe_states++;
		one_iter_mults += choice_starts[adv_starts[i] + 1] - choice_starts[adv_starts[i]];
	}

	adv_starts[n] = row_starts[n];
	numTransitions = choice_starts[row_starts[n]];
	int tot_mults, good_mults;
	terminate = false;

	int *dns_starts = new int[n + 1];
	int *dns_cols = new int[dtmc_max_trans];
	double *dns_nnz = new double[dtmc_max_trans];
	int *inp_dns_starts = new int[n + 1];
	int *inp_dns_cols;
	double *inp_dns_nnz;
	double *in_probs = new double[n];

	// get setup time
	stop = util_cpu_time();
	time_for_setup = (double)(stop - start2)/1000;
	start2 = stop;
	start3 = stop;
	double high_trans, change_sum, effic1, effic2;
	total_mults = 0;
	while(!terminate)
	{	 
		done = false; 
		left = 0;
		for(i = 0; i < n; i++)
			in_probs[i] = 0;
		for(i = 0; i < n; i++)
		{
			if(row_starts[i] >= row_starts[i+1])
				continue;

			l2 = choice_starts[adv_starts[i]];
			h2 = choice_starts[1+adv_starts[i]];
			for(k = l2; k < h2; k++)
				in_probs[cols[k]] += non_zeros[k];
		}
		tot_mults = good_mults = 0;
		high_trans = 0;
		right = 0;
		for(i = 0; i < n; i++)
		{	
			dns_starts[i] = left;
			if(row_starts[i] >= row_starts[i+1])
				continue;

			l2 = choice_starts[adv_starts[i]];
			h2 = choice_starts[1+adv_starts[i]];
			
			threshold = 1.0;
			if(in_probs[i] > 4)
				threshold /= 4;
			else if(in_probs[i] < .25)
				threshold /= .25;
			else
				threshold /= in_probs[i];
			
			x = threshold / (h2 - l2);
			
			for(k = l2; k < h2; k++)
				if(non_zeros[k] >= x)
					right++; 
		}
		inp_dns_cols = new int[right+10];
		inp_dns_nnz = new double[right+10];
		
		for(i = 0; i < n; i++)
		{	
			dns_starts[i] = left;
			if(row_starts[i] >= row_starts[i+1])
				continue;

			l2 = choice_starts[adv_starts[i]];
			h2 = choice_starts[1+adv_starts[i]];
			right = left + h2 - l2 - 1;
			temp_left = right + 1;

			threshold = 1.0;
			if(in_probs[i] > 4)
				threshold /= 4;
			else if(in_probs[i] < .25)
				threshold /= .25;
			else
				threshold /= in_probs[i];
			
			x = threshold / (h2 - l2);
			tot_mults += h2 - l2;
			for(k = l2; k < h2; k++)
				if(non_zeros[k] < x)
				{
					dns_cols[left] = cols[k];
					dns_nnz[left++] = non_zeros[k];					
				}
				else
				{	
					high_trans += non_zeros[k];
					good_mults++;
					dns_cols[right] = cols[k];
					dns_nnz[right--] = non_zeros[k];							
				}
			high_start[i] = right + 1;
			left = temp_left;
		}
		dns_starts[n] = left;
		left = 0;
		for(i = 0; i < n; i++)
		{
			inp_dns_starts[i] = left;
			if(row_starts[i] >= row_starts[i+1])
				continue;
			for(k = high_start[i]; k < dns_starts[1+i]; k++)
			{
				inp_dns_cols[left] = dns_cols[k];
				inp_dns_nnz[left++] = dns_nnz[k];
			}
		}
		inp_dns_starts[n] = left;
		localitr = 0;
		flag = true;
		while(!done && localitr < 100)
		{
			iters++;
			localitr++;
			sup_norm = change_sum = 0;
			if(flag)
			for(i = 0; i < n; i++)
			{	
				d2 = d1 = 0;
				self = 1;
				if(row_starts[i] >= row_starts[i+1])
					continue;
				l2 = dns_starts[i];
				h2 = dns_starts[i+1];
				for(k = l2; k < high_start[i]; k++)
					if(dns_cols[k] != i)
						d1 += dns_nnz[k] * soln[dns_cols[k]];
					else
						self -= dns_nnz[k];
				low_soln[i] = d1;
				low_self[i] = self;

				for(k = high_start[i]; k < h2; k++)
					if(dns_cols[k] != i)
						d1 += dns_nnz[k] * soln[dns_cols[k]];
					else
						self -= dns_nnz[k];
				
				if(self > 0)d1 /= self;else d1 = soln[i];

				x = (d1 - soln[i]);
				change_sum += x;
				soln[i] = d1;
				if (term_crit == TERM_CRIT_RELATIVE && x > 0) 
					x /= soln[i];
				
				if (x > sup_norm) 
					sup_norm = x;				
			}
			else
			for(i = 0; i < n; i++)
			{	
				d3 = d2 = d1 = 0;
				self = 1;
				if(row_starts[i] >= row_starts[i+1])
					continue;				
				
				d1 = low_soln[i];
				self = low_self[i];

				for(k = inp_dns_starts[i]; k < inp_dns_starts[i+1]; k++)
					if(inp_dns_cols[k] != i)
						d3 += inp_dns_nnz[k] * soln[inp_dns_cols[k]];
					else
						self -= inp_dns_nnz[k];
				
				d1 += d3;

				if(self > 0)d1 /= self;else d1 = soln[i];

				x = (d1 - soln[i]);	
				change_sum += x;
				soln[i] = d1;
				if (term_crit == TERM_CRIT_RELATIVE && x > 0) 
					x /= soln[i];
				
				if (x > sup_norm) 
					sup_norm = x;
			}
			if(flag)
			{	
				effic1 = change_sum / tot_mults;
				flag = false;
				if(sup_norm < term_crit_param && localitr > 20) 
					done = true;
				total_mults += one_iter_mults;
			}
			else
			{
				effic2 = change_sum / good_mults;
				if(sup_norm > 1e-6)
					threshold = 1.5;
				else
					threshold = 2;
				if(effic2 / effic1 < threshold)
					flag = true;
				total_mults += good_mults;
			}			
		}	
		delete inp_dns_cols;// = new int[dtmc_max_trans];
		delete inp_dns_nnz;// = new double[dtmc_max_trans];

		one_iter_mults = 0;
 		iters++;
		sup_norm = 0.0;
		// do matrix multiplication and min/max
		h1 = h2 = 0;
		if(iters >  max_iters)
			printf("\nOut of Max iter. Can not converge to the solution after this number of iterations. ");
		else
		for(i = 0; i < n; i++) 
		{
			d1 = 0.0; // initial value doesn't matter
			first = true; // (because we also remember 'first')
			l1 = row_starts[i]; h1 = row_starts[i+1]; 
		
			if(l1 >= h1) continue;									
			for (j = l1; j < h1; j++)
			{
				d2 = 0;
				l2 = choice_starts[j]; h2 = choice_starts[j+1]; 
				
				for (k = l2; k < h2; k++) {
					d2 += non_zeros[k] * soln[cols[k]];
				}
				if (first || (min&&(d2<d1)) || (!min&&(d2-d1> 0-1e-16))) {
					d1 = d2;
					adv_temp = j;
					first = false;				
				}
			}
			
			x = fabs(d1 - soln[i]);
			soln[i] = d1;
			if (term_crit == TERM_CRIT_RELATIVE) {
				x /= soln[i];
			}
			if (x > sup_norm) sup_norm = x;	
			adv_starts[i] = adv_temp;	
			one_iter_mults += choice_starts[adv_starts[i] + 1] - choice_starts[adv_starts[i]];
		}
		total_mults += choice_starts[row_starts[n]];
		if (sup_norm < term_crit_param) 
			done=terminate = true;
		// print occasional status update
		if ((util_cpu_time() - start3) > UPDATE_DELAY) {
			PS_PrintToMainLog(env, "Iteration %d: max %sdiff=%f", iters, (term_crit == TERM_CRIT_RELATIVE)?"relative ":"", sup_norm);
			PS_PrintToMainLog(env, ", %.2f sec so far\n", ((double)(util_cpu_time() - start2)/1000));
			start3 = util_cpu_time();
		}
	}
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
	if (a) Cudd_RecursiveDeref(ddman, a);
	if (ndsm) delete ndsm;
	if (yes_vec) delete[] yes_vec;
	if (strat == NULL && adv) delete[] adv;
	if (action_names != NULL) {
		release_string_array_from_java(env, action_names_jstrings, action_names, num_actions);
	}
			
	printf("\n\nNumber of scallar multiplications: %dM \n", (int) (total_mults / 1000000));
	return ptr_to_jlong(soln);
}

//------------------------------------------------------------------------------


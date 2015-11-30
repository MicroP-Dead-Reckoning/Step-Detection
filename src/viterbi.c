#include <stdio.h>
#include <stdlib.h>

#include "viterbi.h"

typedef struct {
	int S;
	int V;
	float transition[S_DEF][S_DEF];
	float emission[S_DEF][V_DEF];
	float prior[S_DEF];
} hmm_desc;

int ViterbiUpdate_c(float* viterbi_in, float* viterbi_out, int obs, hmm_desc *model);
int Viterbi_C(float* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm);

float states[N_OBS_TAKEN+1][2*S_DEF];
float EstimatedStates[N_OBS_TAKEN];
int acceleratometer_observations[9];
int acceleratometer_states[9];

float alpha1 = 0.7;
float alpha2 = 0.8;

int N[3] = {3, 3, 3};

hmm_desc hmm = {
	S_DEF, 
	V_DEF,
	{
		{0.09,0.9,0.09,0.05},
		{0.9,0.01,0.45,0.5},
		{0.01,0.9,0.01,0},
		{0,0,0.45,0}
	},
	{
		{0,0,0,1},
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0}
	},
	{0.25,0.25,0.25,0.25}
};

/*!
	ReadAccelerometer takes in an array of accelerometer data and gets observation
	values based off the accelerometer readings
										
	\param data
	\param data_len size of data
	\param observation output vector containing observed data.
	\param nObs	size of observation	
*/										
int ReadAccelerometer(float* data, int data_len, int* observation, int* nObs) {
	int ct[3] = {0, 0, 0};
	int last_entry = -1;
	int current_state = 0;
	*nObs = 0;
	int z;
	for (int i=0; i < data_len; i++) {
		// Determine current state
		if (-1*alpha1 < data[i] && data[i] < alpha1) {
			z = 0;
		} 
		else if ( data[i] > alpha2) {
			z = 1;
		} 
		else if ( data[i] < -1 * alpha2) {
			z = 2;
		} 
		else z = -1;
		if (z != -1) {
			// resets count vector if new observation
			if (z != current_state) {
				ct[current_state] = 0;
				current_state = z;
			}
			ct[z]++;
			// adds observation if more than N entries
			if (ct[z] >= N[z] && z != last_entry) {
				observation[*nObs] = z;
				last_entry = z;
				(*nObs)++;
				printf("%d\t", last_entry);
			}
		}
	}
	printf("\n");
	return 0;
}
/*!
	Calls tests in order:
1. Test 1
2. ReadAccelerometer
3. Test 2

There were various test cases which we did in the demo. We simply commented out
each test case instead of deleting them.

*/
int viterbi_update(float* data)
{	
	int EstimatedStates[N_OBS_TAKEN];
	printf("ESTIMATED STATES TEST 1\n");
	printf("=======================\n");
	Viterbi_C(data, N_OBS_TAKEN, EstimatedStates, &hmm);
	return 0;
}

/*!
	\param Observations Observed states, used with emissions 
	\param Nobs size of Observations
	\param EstimatedStates output containing the EstimatedStates at all times
	\param HMM hidden markov model used for data input
*/
int Viterbi_C(float* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm) {
	// determine starting states
	for (int i =0; i < hmm->S; i++) {
		states[0][2*i] = hmm->prior[i] * hmm->emission[i][Observations[0]];
		states[0][2*i+1] = 0;
		//printf("%f = %f * %f\n", states[0][2*i], hmm->prior[i], hmm->emission[i][Observations[0]]);
	}
	// Determine states for each time
	for (int i = 1; i < Nobs+1; i++) {
		ViterbiUpdate_c(states[i-1], states[i], Observations[i], hmm);
	}
	// determine maximum  probability states for all observation
	for (int i = 1; i < Nobs+1; i++) {
		float max = -1;
		for (int j = 0; j < S_DEF; j++) {
			//printf("(%f, %f)\t", states[i][2*j], states[i][2*j+1]);
			if (states[i][2*j] > max) {
				max = states[i][2*j];
				EstimatedStates[i-1] = (int) states[i][2*j+1];
			}
		}
		//printf("\n");
	}
	// print maximum probability state for each observation
	for (int i = 0; i < Nobs; i++) {
		printf("%d\t", EstimatedStates[i]);
	}
	printf("\n");
	return 0;
}

/*!
	ViterbitUpdate_c 
	\param viterbi_in contains the previous state information, in the form (vit, psi)
	\param viterbi_out returns the current state information, in the form (vit, psi)
	\param obs observation at the current time
	\param model Hidden Markov Model at current time
	
	Using the previous viterbi state information and their associated HMM, this function
	determines the current viterbi state information
*/
int ViterbiUpdate_c(float* viterbi_in, float* viterbi_out, int obs, hmm_desc *model) {
	int max_index = -1;
	float trans_p[S_DEF];
	for (int i = 0; i < model->S; i++) {
		// find the transition probability for state i
		for (int j = 0; j < model->S; j++) {
			trans_p[j] = viterbi_in[2*j] * model->transition[j][i];
		}
		// find the maximum probability
		float max = -1000;
		for (int j = 0; j < model->S; j++) {
			if (trans_p[j] > max) {
				max = trans_p[j];
				max_index = j;
			}
		}
		// update viterbi vector
		viterbi_out[2*i] = max;						// assign vit (value)
		//printf("%s: %3.3f\n", PRINT_STATE[i], max);
		viterbi_out[2*i + 1] = max_index; // assign psi (index)
		viterbi_out[2*i] = max * model->emission[i][obs];
	}
	
	// normalize viterbi vector 
	float sum = 0;
	for (int i = 0; i < model->S; i++) {
		sum += viterbi_out[2*i];
	}
	
	float scale = 1.0f/sum;
	
	for (int i = 0; i < model->S; i++) {
		viterbi_out[2*i] = viterbi_out[2*i] * scale;
	}
	
	return 0;
}

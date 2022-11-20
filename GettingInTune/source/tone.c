/*
 * tone.c
 *
 *  Created on: Nov 19, 2022
 *      Author: dayton.flores
 */

#include <stdio.h>
#include "board.h"
#include "fp_trig.h"
#include "tone.h"

/**
 * \def		SAMPLE_RATE_DAC_HZ
 * \brief	The sampling rate for DAC in Hz
 */
#define SAMPLE_RATE_DAC_HZ\
	(48000)

/**
 * \def		SIN_MIN_DEGREES
 * \brief	The min degrees for sin function
 */
#define SIN_MIN_DEGREES\
	(0)

/**
 * \def		SIN_MAX_DEGREES
 * \brief	The max degrees for sin function
 */
#define SIN_MAX_DEGREES\
	(360)

/**
 * \def		A4_HZ
 * \brief	The frequency of tone A4 in Hz
 */
#define A4_HZ\
	(440)

/**
 * \def		D5_HZ
 * \brief	The frequency of tone D5 in Hz
 */
#define D5_HZ\
	(587)

/**
 * \def		E5_HZ
 * \brief	The frequency of tone E5 in Hz
 */
#define E5_HZ\
	(659)

/**
 * \def		A5_HZ
 * \brief	The frequency of tone A5 in Hz
 */
#define A5_HZ\
	(880)

/**
 * \def		SAMPLES_PER_PERIOD_A4
 * \brief	The amount of samples per period for tone A4
 * \detail
 * 		This is calculated by:
 * 			= SAMPLE_RATE_DAC_HZ / A4_HZ
 * 			= 48000 / 440
 * 			= 109
 */
#define SAMPLES_PER_PERIOD_A4\
	(109)

/**
 * \def		SAMPLES_PER_PERIOD_D5
 * \brief	The amount of samples per period for tone D5
 * \detail
 * 		This is calculated by:
 * 			= SAMPLE_RATE_DAC_HZ / D5_HZ
 * 			= 48000 / 587
 * 			= 81
 */
#define SAMPLES_PER_PERIOD_D5\
	(81)

/**
 * \def		SAMPLES_PER_PERIOD_E5
 * \brief	The amount of samples per period for tone E5
 * \detail
 * 		This is calculated by:
 * 			= SAMPLE_RATE_DAC_HZ / E5_HZ
 * 			= 48000 / 659
 * 			= 72
 */
#define SAMPLES_PER_PERIOD_E5\
	(72)

/**
 * \def		SAMPLES_PER_PERIOD_A5
 * \brief	The amount of samples per period for tone A5
 * \detail
 * 		This is calculated by:
 * 			= SAMPLE_RATE_DAC_HZ / A5_HZ
 * 			= 48000 / 880
 * 			= 54
 */
#define SAMPLES_PER_PERIOD_A5\
	(54)

/**
 * \var		samples_a4
 * \brief	Buffer to hold audio out samples for 1 period of tone A4
 */
int32_t samples_a4[SAMPLES_PER_PERIOD_A4];

/**
 * \var		samples_d5
 * \brief	Buffer to hold audio out data for tone D5
 */
int32_t samples_d5[SAMPLES_PER_PERIOD_D5];

/**
 * \var		samples_e5
 * \brief	Buffer to hold audio out data for tone E5
 */
int32_t samples_e5[SAMPLES_PER_PERIOD_E5];

/**
 * \var		samples_a5
 * \brief	Buffer to hold audio out data for tone A5
 */
int32_t samples_a5[SAMPLES_PER_PERIOD_A5];

/**
 * \var		dac_buffer
 * \brief	Buffer to hold samples for DMA's source
 */
int32_t dac_buffer[DAC_BUF_SIZE];

/**
 * \var		dac_buffer_full_periods
 * \brief	Tracks how many full periods are contained in dac_buffer
 */
int32_t dac_buffer_full_periods;

void tone_to_samples(void)
{
	/**
	 * Calculate the amount of degrees to step (starting from 0 to 360)
	 * for tones A4, D5, E5, A5
	 */
	float step_a4 = (SIN_MAX_DEGREES - SIN_MIN_DEGREES) / ((float)SAMPLES_PER_PERIOD_A4);
	float step_d5 = (SIN_MAX_DEGREES - SIN_MIN_DEGREES) / ((float)SAMPLES_PER_PERIOD_D5);
	float step_e5 = (SIN_MAX_DEGREES - SIN_MIN_DEGREES) / ((float)SAMPLES_PER_PERIOD_E5);
	float step_a5 = (SIN_MAX_DEGREES - SIN_MIN_DEGREES) / ((float)SAMPLES_PER_PERIOD_A5);
	float deg_a4 = 0;
	float deg_d5 = 0;
	float deg_e5 = 0;
	float deg_a5 = 0;
	int largest_buffer_size = SAMPLES_PER_PERIOD_A4;

	for(int i = 0; i < largest_buffer_size; i++){

		/**
		 * Calculate sin for 1st period of A4. Then mirror 1st period for
		 * the rest of the buffer
		 */
		if(i < SAMPLES_PER_PERIOD_A4){
			samples_a4[i] = fp_sin(fp_radians((uint32_t)(deg_a4)));
			deg_a4 += step_a4;
			//printf("a4 %d %d\r\n", i, samples_a4[i]);
		}

		/**
		 * Calculate sin for 1st period of D5. Then mirror 1st period for
		 * the rest of the buffer
		 */
		if(i < SAMPLES_PER_PERIOD_D5){
			samples_d5[i] = fp_sin(fp_radians((uint32_t)(deg_d5)));
			deg_d5 += step_d5;
			//printf("d5 %d %d\r\n", i, samples_d5[i]);
		}

		/**
		 * Calculate sin for 1st period of E5. Then mirror 1st period for
		 * the rest of the buffer
		 */
		if(i < SAMPLES_PER_PERIOD_E5){
			samples_e5[i] = fp_sin(fp_radians((uint32_t)(deg_e5)));
			deg_e5 += step_e5;
			//printf("e5 %d %d\r\n", i, samples_e5[i]);
		}

		/**
		 * Calculate sin for 1st period of A5. Then mirror 1st period for
		 * the rest of the buffer
		 */
		if(i < SAMPLES_PER_PERIOD_A5){
			samples_a5[i] = fp_sin(fp_radians((uint32_t)(deg_a5)));
			deg_a5 += step_a5;
			//printf("a5 %d %d\r\n", i, samples_a5[i]);
		}
	}
}

void fill_dac_buffer(tone_t tone)
{
	int i;
	int j;
	dac_buffer_full_periods = 0;
	switch(tone){

	/**
	 * Copy A4 samples into DAC buffer until it is filled up
	 */
	case A4:
		for(i = 0, j = 0; i < DAC_BUF_SIZE; i++, j++){
			if(j >= SAMPLES_PER_PERIOD_A4){
				j = 0;
				dac_buffer_full_periods++;
			}

			dac_buffer[i] = samples_a4[j];
			//printf("a4 %d %d\r\n", i, dac_buffer[i]);
		}
		break;

	/**
	 * Copy D5 samples into DAC buffer until it is filled up
	 */
	case D5:
		for(i = 0, j = 0; i < DAC_BUF_SIZE; i++, j++){
			if(j >= SAMPLES_PER_PERIOD_D5){
				j = 0;
				dac_buffer_full_periods++;
			}

			dac_buffer[i] = samples_d5[j];
			//printf("d5 %d %d\r\n", i, dac_buffer[i]);
		}
		break;

	/**
	 * Copy E5 samples into DAC buffer until it is filled up
	 */
	case E5:
		for(i = 0, j = 0; i < DAC_BUF_SIZE; i++, j++){
			if(j >= SAMPLES_PER_PERIOD_E5){
				j = 0;
				dac_buffer_full_periods++;
			}

			dac_buffer[i] = samples_e5[j];
			//printf("e5 %d %d\r\n", i, dac_buffer[i]);
		}
		break;

	/**
	 * Copy A5 samples into DAC buffer until it is filled up
	 */
	case A5:
		for(i = 0, j = 0; i < DAC_BUF_SIZE; i++, j++){
			if(j >= SAMPLES_PER_PERIOD_A5){
				j = 0;
				dac_buffer_full_periods++;
			}

			dac_buffer[i] = samples_a5[j];
			//printf("a5 %d %d\r\n", i, dac_buffer[i]);
		}
		break;

	/**
	 * Undefined
	 */
	default:
		break;
	}
}

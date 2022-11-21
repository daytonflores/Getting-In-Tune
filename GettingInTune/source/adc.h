/*
 * adc.h
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#ifndef ADC_H_
#define ADC_H_

/**
 * \def		SAMPLE_RATE_ADC_HZ
 * \brief	The sampling rate for ADC in Hz
 */
#define SAMPLE_RATE_ADC_HZ\
	(96000)

/**
 * \def		SAMPLE_PERIOD_ADC_US
 * \brief	The sampling period for ADC in us
 */
#define SAMPLE_PERIOD_ADC_US\
	(1000000.0 / SAMPLE_RATE_ADC_HZ)

/**
 * \def		SC1_ADCH
 * \brief	SC1[4:0] which is Input Channel Select
 * \detail
 * 		10111: When DIFF is cleared, AD23 (PTE30) is selected as input.
 * 			   When DIFF is set, this is reserved
 */
#define SC1_ADCH\
	(23)

/**
 * \fn		void init_onboard_adc
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the on-board ADC
 */
void init_onboard_adc(void);

#endif /* ADC_H_ */

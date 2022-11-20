/*
 * dac.h
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#ifndef DAC_H_
#define DAC_H_

/**
 * \def		SAMPLE_RATE_DAC_HZ
 * \brief	The sampling rate for DAC in Hz
 */
#define SAMPLE_RATE_DAC_HZ\
	(48000)

/**
 * \def		SAMPLE_PERIOD_DAC_US
 * \brief	The sampling period for DAC in us
 */
#define SAMPLE_PERIOD_DAC_US\
	(1000000.0 / SAMPLE_RATE_DAC_HZ)

/**
 * \fn		void init_onboard_dac
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the on-board DAC
 */
void init_onboard_dac(void);

#endif /* DAC_H_ */

/*
 * tone.h
 *
 *  Created on: Nov 19, 2022
 *      Author: dayton.flores
 */

#ifndef TONE_H_
#define TONE_H_

/**
 * \def		DAC_BUF_SIZE
 * \brief	Size of the DAC sample output buffer for each tone
 */
#define DAC_BUF_SIZE\
	(1024)

/**
 * \def		ADC_BUF_SIZE
 * \brief	Size of the ADC sample buffer for each tone
 */
#define ADC_BUF_SIZE\
	(1024)

/**
 * \typedef	typedef enum tone_e tone_t
 * \brief   Easily declare musical tones
 */
typedef enum tone_e tone_t;

/**
 * \enum	enum tone_e
 * \brief   To reference musical tones
 */
enum tone_e{
	A4,
	D5,
	E5,
	A5
};

/**
 * \var		dac_buffer
 * \brief	Defined in tone.c
 */
extern int16_t dac_buffer[DAC_BUF_SIZE];
extern int16_t dac_buffer_test[DAC_BUF_SIZE];

/**
 * \var		dac_buffer_samples_per_period
 * \brief	Defined in tone.c
 */
extern int32_t dac_buffer_samples_per_period;

/**
 * \var		dac_buffer_full_periods
 * \brief	Defined in tone.c
 */
extern int32_t dac_buffer_full_periods;

/**
 * \var		dac_buffer_hz
 * \brief	Defined in tone.c
 */
extern int32_t dac_buffer_hz;

/**
 * \var		dac_buffer_samples
 * \brief	Defined in tone.c
 */
extern int32_t dac_buffer_samples;

/**
 * \var		adc_buffer
 * \brief	Defined in tone.c
 */
extern int16_t adc_buffer[ADC_BUF_SIZE];

/**
 * \var		adc_buffer_i
 * \brief	Defined in tone.c
 */
extern int32_t adc_buffer_i;

/**
 * \var		volatile bool adc_done
 * \brief	Defined in tone.c
 */
extern volatile bool adc_done;

/**
 * \fn		void tone_to_samples
 * \param	N/A
 * \return	N/A
 * \brief   Computes samples representing pure tone of specified frequency,
 * 			based on sin(x) (in fp_trig.o). The tones that will be computed
 * 			are A4 (440 Hz), D5 (587 Hz), E5 (659 Hz), A5 (880 Hz). Each tone
 * 			will have its own output buffer that will get pre-computed
 */
void tone_to_samples(void);

/**
 * \fn		void fill_dac_buffer
 * \param	tone_t tone
 * \return	N/A
 * \brief   Stuffs DAC buffer with parameter tone's samples until
 * 			DAC buffer is filled up
 */
void fill_dac_buffer(tone_t tone);

/**
 * \fn		void fill_adc_buffer
 * \param	N/A
 * \return	N/A
 * \brief   Fills ADC buffer with readings from ADC
 */
void fill_adc_buffer(void);

#endif /* TONE_H_ */

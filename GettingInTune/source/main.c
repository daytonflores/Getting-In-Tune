/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "adc.h"
#include "autocorrelate.h"
#include "dac.h"
#include "dma.h"
#include "fp_trig.h"
#include "systick.h"
#include "test_sine.h"
#include "tone.h"
#include "tpm.h"

/**
 * \var		current_tone
 * \brief	The current_tone to be send out to DAC
 */
tone_t current_tone = A4;

/**
 * \fn		int main
 * \param	N/A
 * \return	EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 * \brief   Application entry point
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /**
     * Test sin function generated from given fp_trig.o
     */
    test_sin();
    printf("\n");

    /**
     * Pre-compute samples for all tones
     */
    tone_to_samples();

    /**
     * Stuff DAC buffer with initial tone's samples until DAC buffer is full
     */
    fill_dac_buffer(current_tone);

    /**
     * Initialize on-board DAC
     */
    init_onboard_dac();

    /**
     * Initialize on-board ADC
     */
    init_onboard_adc();

    /**
     * Initialize on-board DMA
     */
    init_onboard_dma();

    /**
     * Initialize on-board TPM for use with DMA
     */
    init_onboard_tpm(SAMPLE_PERIOD_DAC_US, SAMPLE_PERIOD_ADC_US);

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Print info about current tone
     */
    printf("Generated %d samples at %d Hz. Computed period = %d samples\r\n",
    		dac_buffer_full_periods * dac_buffer_samples_per_period,
			dac_buffer_hz,
			dac_buffer_samples_per_period);

    /**
     * Begin TPM counter
     */
    start_onboard_tpm();

    /**
     * Begin DMA transfer
     */
    start_onboard_dma((uint16_t*)dac_buffer, dac_buffer_samples << 1);

    /**
     * Begin reading samples from ADC
     */
    int32_t adc_min = 0;
    int32_t adc_max = 0;
    int32_t adc_avg = 0;

    /**
     * Main infinite loop
     */
    while(1) {

    	/**
    	 * If we still have samples to read
    	 */
    	if(!adc_done){

        	/**
        	 * Reset ADC buffer index and print since 1024 samples were taken
        	 */
        	if(adc_buffer_i >= ADC_BUF_SIZE){
        		adc_done = true;
        		adc_buffer_i = 0;
        	    printf("min = %d, max = %d, avg = %d, period = %d samples, frequency = %d Hz\r\n\n",
        	    		adc_min,
						adc_max,
						(adc_avg >> 10),
						((autocorrelate_detect_period(adc_buffer, ADC_BUF_SIZE, kAC_16bps_unsigned)) >> 1),
						(SAMPLE_RATE_ADC_HZ / autocorrelate_detect_period(adc_buffer, ADC_BUF_SIZE, kAC_16bps_unsigned)) << 1);
        	    adc_min = 0;
        	    adc_max = 0;
        	    adc_avg = 0;
        	}

            /**
             * Begin reading a sample from ADC
             */
            ADC0->SC1[0] = ADC_SC1_ADCH(SC1_ADCH);
        	while(!(ADC0->SC1[0] & ADC_SC1_COCO(1)));
        	adc_buffer[adc_buffer_i] = ((int16_t)(ADC0->R[0]));
        	adc_avg += adc_buffer[adc_buffer_i];
        	if(adc_buffer[adc_buffer_i] < adc_min){
        		adc_min = adc_buffer[adc_buffer_i];
        	}
        	if(adc_buffer[adc_buffer_i] > adc_max){
        		adc_max = adc_buffer[adc_buffer_i];
        	}
        	adc_buffer_i++;
    	}

        /**
         * Set by SysTick_Handler every TICK_SEC
         */
        if(tick){

            /**
             * Reset flag that was set by SysTick ISR
             */
        	tick = false;

            /**
             * Increment for timestamp purposes
             */
        	ticks_since_startup++;
        	//printf("Sec since startup = %f\r\n", ticks_since_startup * TICK_SEC);

            /**
             * Increment for note timing purposes
             */
        	ticks_since_last_note++;
    		//printf("Sec since last note = %f\r\n", ticks_since_last_note * TICK_SEC);

        	/**
        	 * Check if a full second has passed since current note started
        	 */
        	if(!(ticks_since_last_note & (TICK_HZ - 1))){

                /**
                 * Reset for note timing purposes
                 */
        		ticks_since_last_note = 0;

        		/**
        		 * Change note
        		 */
        		switch(current_tone){
        		case A4:
        			current_tone = D5;
        			break;
        		case D5:
        			current_tone = E5;
        			break;
        		case E5:
        			current_tone = A5;
        			break;
        		case A5:
        			current_tone = A4;
        			break;
        		default:
        			break;
        		}

        	    /**
        	     * Stuff DAC buffer with tone's samples until DAC buffer is full
        	     */
        	    fill_dac_buffer(current_tone);

        	    /**
        	     * Print info about current tone
        	     */
        	    printf("Generated %d samples at %d Hz. Computed period = %d samples\r\n",
        	    		dac_buffer_samples,
						dac_buffer_hz,
						dac_buffer_samples_per_period);

        	    /**
        	     * Begin DMA transfer
        	     */
        	    start_onboard_dma((uint16_t*)dac_buffer, dac_buffer_samples << 1);

        	    /**
        	     * Begin ADC sampling for the new current tone
        	     */
        		adc_done = false;
        	}
        }


    }
    return 0 ;
}

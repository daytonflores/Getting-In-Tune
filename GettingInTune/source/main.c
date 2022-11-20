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
#include "dac.h"
#include "dma.h"
#include "fp_trig.h"
#include "systick.h"
#include "test_sine.h"
#include "tone.h"
#include "tpm.h"

/**
 * \def		BUF_ADC_SIZE
 * \brief	Size of audio out buffer
 */
#define BUF_ADC_SIZE\
	(1)

/**
 * \def		SAMPLE_COUNT_ADC
 * \brief	The number of samples to transfer at a time
 */
#define SAMPLE_COUNT_ADC\
	(100)

/**
 * \var		buf_adc
 * \brief	Buffer to hold audio out data
 */
uint16_t buf_adc[BUF_ADC_SIZE];

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
    init_onboard_dma(buf_adc, SAMPLE_COUNT_ADC);

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_tpm();

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Test sin function generated from given fp_trig.o
     */
    //test_sin();

    /**
     * Pre-compute samples for all tones
     */
    tone_to_samples();

    /**
     * Stuff DAC buffer with initial tone's samples until DAC buffer is full
     */
    fill_dac_buffer(current_tone);

    /**
     * Print info about current tone
     */
    printf("Generated %d samples at %d Hz. Computed period = %d samples\r\n",
    		dac_buffer_full_periods * dac_buffer_samples_per_period,
			dac_buffer_hz,
			dac_buffer_samples_per_period);

    /**
     * Main infinite loop
     */
    while(1) {

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
        	    		dac_buffer_full_periods * dac_buffer_samples_per_period,
						dac_buffer_hz,
						dac_buffer_samples_per_period);
        	}
        }
    }
    return 0 ;
}

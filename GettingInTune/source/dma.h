/*
 * dma.h
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#ifndef DMA_H_
#define DMA_H_

/**
 * \fn		void init_onboard_dma
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the on-board DMA
 */
void init_onboard_dma(void);

/**
 * \fn		void start_onboard_dma
 * \param	uint16_t *source The beginning of the source data to transfer
 * \param	uint32_t count The number of bytes to transfer at a time
 * \return	N/A
 * \brief   Start the on-board DMA
 */
void start_onboard_dma(uint16_t *source, uint32_t count);

/**
 * \fn		void DMA0_IRQHandler
 * \param	N/A
 * \return	N/A
 * \brief   The ISR for the DMA transfer (i.e. runs each time the DMA
 * 			has transferred all data).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void DMA0_IRQHandler(void);

#endif /* DMA_H_ */

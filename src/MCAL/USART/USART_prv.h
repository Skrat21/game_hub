/*
 * USART_prv.h
 *
 *  Created on: Aug 31, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_USART_USART_PRV_H_
#define MCAL_USART_USART_PRV_H_

#include "../../LIB/STD_TYPES.h"

#define USART1_BASE_ADDR 0x40011000U
#define USART2_BASE_ADDR 0x40004400U
#define USART6_BASE_ADDR 0x40011400U


typedef struct{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 GTPR;
}USART_MemMap_t;

#define USART1	((USART_MemMap_t*)(USART1_BASE_ADDR))
#define USART2	((USART_MemMap_t*)(USART2_BASE_ADDR))
#define USART6	((USART_MemMap_t*)(USART6_BASE_ADDR))

#endif /* MCAL_USART_USART_PRV_H_ */

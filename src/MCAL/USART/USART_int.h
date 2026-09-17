/*
 * USART_int.h
 *
 *  Created on: Aug 31, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_USART_USART_INT_H_
#define MCAL_USART_USART_INT_H_

#include "../../LIB/STD_TYPES.h"


void MUSART_vInit();
u8 MUSART_u8SendData(u8 A_u8Data);
void MUSART_vSendData(u8 A_u8Data);
u8 MUSART_u8RecieveData(void);

// Interrupts

void MUSART_vEnable_TX_Interrupt(void);
void MUSART_vDisable_TX_Interrupt(void);
void MUSART_vEnable_TC_Interrupt(void);
void MUSART_vDisable_TC_Interrupt(void);
void MUSART_vEnable_RX_Interrupt(void);
void MUSART_vDisable_RX_Interrupt(void);

//TODO
void MUSART_vSendString(char *A_u8ptrStr);
u8 MUSART_u8IsStringReady(void);
u8* MUSART_u8ptrGetReceivedString(void);
u8 MUSART_u8ReadReceivedByte(u8 *data);
void MUSART_vClearReceiveBuffer(void);
void MUSART_vUSARTCallBack(u8 A_u8USARTNo, void (*Fptr)(void));
u8 MUSART_u8Retreive_USART1_DataRegister(void);
void MUSART_vWrite_USART1_DataRegister(u8 A_u8Data);

void MUSART_vSendCharAsync(u8 A_u8Data);
void MUSART_vSendStringAsync(u8 *A_u8ptrStr);



#endif /* MCAL_USART_USART_INT_H_ */

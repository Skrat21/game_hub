#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "USART_int.h"
#include "USART_prv.h"

#include <stdio.h>

#define MAXBUFFERSIZE 1024
#define USART_SEND_TIMEOUT 1000000UL

volatile static u8 G_data;
volatile static u8 u8G_Buffer[MAXBUFFERSIZE];
volatile static u8 G_u8RxQueue[MAXBUFFERSIZE];
volatile static u16 G_u16RxRead = 0;
volatile static u16 G_u16RxWrite = 0;
volatile static u8 *G_u8PtrData = NULL;
volatile static u8 G_u8len = 0;
volatile static int G_u8Indx = 0;
volatile static u8 G_u8Busy = 0;
volatile static u8 G_u8Ready = 0;
volatile static u8 G_u8RxIndx = 0;
volatile static u8 G_u8IgnoreNextLF = 0;

u8 MUSART_u8IsStringReady(void) {
	return G_u8Ready;
}

u8* MUSART_u8ptrGetReceivedString(void) {
	G_u8Ready = 0;
	return (u8*)u8G_Buffer;
}

u8 MUSART_u8ReadReceivedByte(u8 *data) {
	if (data == NULL || G_u16RxRead == G_u16RxWrite) {
		return 0;
	}

	*data = G_u8RxQueue[G_u16RxRead];
	G_u16RxRead = (u16) ((G_u16RxRead + 1) % MAXBUFFERSIZE);
	return 1;
}

void MUSART_vClearReceiveBuffer(void) {
	G_u8Ready = 0;
	G_u8RxIndx = 0;
	G_u8IgnoreNextLF = 0;
	G_u16RxRead = 0;
	G_u16RxWrite = 0;
	u8G_Buffer[0] = '\0';
}

void MUSART_vInit() {
	// OVERSAMPLE BY 16
	CLR_BIT(USART1->CR1, 15);

	// DATA LENGTH 8
	CLR_BIT(USART1->CR1, 12);

	// NO PARITY
	CLR_BIT(USART1->CR1, 10);

	// BAUD RATE 115200: 25 MHz APB2 clock, oversampling by 16
	USART1->BRR = (13 << 4) | 9;

	// 1 STOP BIT

	CLR_BIT(USART1->CR2, 12);
	CLR_BIT(USART1->CR2, 13);

	// ENABLE TRANSMITTER
	SET_BIT(USART1->CR1, 3);

	// ENABLE RECEIVER
	SET_BIT(USART1->CR1, 2);

	// ENABLE USART
	SET_BIT(USART1->CR1, 13);

	// ENABLE USART REXE INTERRUPT
	MUSART_vEnable_RX_Interrupt();
}

void MUSART_vSendData(u8 A_u8Data) {
	(void) MUSART_u8SendData(A_u8Data);
}

u8 MUSART_u8SendData(u8 A_u8Data) {
	volatile u32 timeout = USART_SEND_TIMEOUT;

	while (!GET_BIT(USART1->SR, 7) && timeout > 0) {
		timeout--;
	}
	if (timeout == 0) {
		return 0;
	}
	USART1->DR = A_u8Data;

	timeout = USART_SEND_TIMEOUT;
	while (!GET_BIT(USART1->SR, 6) && timeout > 0) {
		timeout--;
	}
	if (timeout == 0) {
		return 0;
	}

	CLR_BIT(USART1->SR, 6);
	return 1;
}

u8 MUSART_u8RecieveData(void) {
	while (!GET_BIT(USART1->SR, 5))
		;
	return USART1->DR;
}

void MUSART_vEnable_TX_Interrupt(void) {
	SET_BIT(USART1->CR1, 7);
}
void MUSART_vDisable_TX_Interrupt(void) {
	CLR_BIT(USART1->CR1, 7);
}
void MUSART_vEnable_TC_Interrupt(void) {
	SET_BIT(USART1->CR1, 6);
}
void MUSART_vDisable_TC_Interrupt(void) {
	CLR_BIT(USART1->CR1, 6);
}
void MUSART_vEnable_RX_Interrupt(void) {
	SET_BIT(USART1->CR1, 5);
}
void MUSART_vDisable_RX_Interrupt(void) {
	CLR_BIT(USART1->CR1, 5);
}

void MUSART_vSendString(char *A_u8ptrStr) {
	if (A_u8ptrStr != NULL) {
		u8 c = A_u8ptrStr[0];
		int counter = 0;
		while (c != '\0') {
			MUSART_vSendData(c);
			counter++;
			c = A_u8ptrStr[counter];

		}
	}
}

void MUSART_vSendCharAsync(u8 A_u8Data) {
	G_data = A_u8Data;
	MUSART_vEnable_TX_Interrupt();
}

void MUSART_vSendStringAsync(u8 *A_u8ptrStr) {
	while (G_u8Busy)
		;
	G_u8Busy = 1;
	G_u8PtrData = A_u8ptrStr;
	G_u8Indx = 0;
	MUSART_vEnable_TX_Interrupt();
}

void USART1_IRQHandler(void) {
	if ((GET_BIT(USART1->SR,7) == 1) && (GET_BIT(USART1->CR1,7) == 1)) {
		if (G_u8PtrData[G_u8Indx] != '\0' && G_u8Indx < MAXBUFFERSIZE - 1) {
			USART1->DR = G_u8PtrData[G_u8Indx];
			G_u8Indx++;
		} else {
			MUSART_vDisable_TX_Interrupt();
			MUSART_vEnable_TC_Interrupt();
			G_u8Indx = 0;
			G_u8Busy = 0;
		}
	}
	if ((GET_BIT(USART1->SR,6) == 1) && (GET_BIT(USART1->CR1,6) == 1)) {
		CLR_BIT(USART1->SR, 6);
		MUSART_vDisable_TC_Interrupt();
	}
	if ((GET_BIT(USART1->SR,5) == 1) && (GET_BIT(USART1->CR1,5) == 1)) {
		u8 temp = (u8) (USART1->DR & 0xFF); // reading DR clears RXNE automatically
		u16 next_write = (u16) ((G_u16RxWrite + 1) % MAXBUFFERSIZE);

		if (next_write != G_u16RxRead) {
			G_u8RxQueue[G_u16RxWrite] = temp;
			G_u16RxWrite = next_write;
		}

		if (temp == '>') {
			u8G_Buffer[0] = '>';
			u8G_Buffer[1] = '\0';
			G_u8RxIndx = 0;
			G_u8Ready = 1;
			G_u8IgnoreNextLF = 0;
		} else if (temp == '\n' && G_u8IgnoreNextLF) {
			G_u8IgnoreNextLF = 0;
		} else if (temp == '\r') {
			u8G_Buffer[G_u8RxIndx] = '\0';
			G_u8RxIndx = 0;
			G_u8Ready = 1;
			G_u8IgnoreNextLF = 1;
		} else if (temp == '\n') {
			u8G_Buffer[G_u8RxIndx] = '\0';
			G_u8RxIndx = 0;
			G_u8Ready = 1;
		} else if (G_u8RxIndx < (MAXBUFFERSIZE - 1)) {
			G_u8IgnoreNextLF = 0;
			u8G_Buffer[G_u8RxIndx++] = temp;
		}
		// else: buffer full, byte silently dropped until a terminator arrives
	}
}

u8* MUSART_u8ptprReceiveString(void) // check '\r' | '\n'
{
	//while (G_u8Busy);

	//G_u8Busy = 1;
	G_u8Indx = 0;
	while (1) {
		char temp = MUSART_u8RecieveData();
		if (temp == '\r' || temp == '\n')
			break;
		else {
			u8G_Buffer[G_u8Indx] = temp;
			G_u8Indx = (G_u8Indx + 1) % 50;
		}
	}
	u8G_Buffer[G_u8Indx] = '\0';
	return u8G_Buffer;
}
void MUSART_vUSARTCallBack(u8 A_u8USARTNo, void (*Fptr)(void));
u8 MUSART_u8Retreive_USART1_DataRegister(void);
void MUSART_vWrite_USART1_DataRegister(u8 A_u8Data);


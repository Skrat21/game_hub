/*
 * 7_SEG_prg.c
 *
 *  Created on: Aug 21, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_7_SEG_7_SEG_PRG_C_
#define MCAL_7_SEG_7_SEG_PRG_C_

#include "../../MCAL/GPIO/GPIO_int.h"
#include "SEG7_int.h"

/* 7-Segment order: { A, B, C, D, E, F, G, DP } */

const u8 SEG7_Alphabet[26][8] = {
/* A */{ 1, 1, 1, 0, 1, 1, 1, 0 },
/* B */{ 0, 0, 1, 1, 1, 1, 1, 0 },
/* C */{ 1, 0, 0, 1, 1, 1, 0, 0 },
/* D */{ 0, 1, 1, 1, 1, 0, 1, 0 },
/* E */{ 1, 0, 0, 1, 1, 1, 1, 0 },
/* F */{ 1, 0, 0, 0, 1, 1, 1, 0 },
/* G */{ 1, 0, 1, 1, 1, 1, 0, 0 },
/* H */{ 0, 1, 1, 0, 1, 1, 1, 0 },
/* I */{ 0, 1, 1, 0, 0, 0, 0, 0 },
/* J */{ 0, 1, 1, 1, 0, 0, 0, 0 },
/* K */{ 1, 0, 1, 0, 1, 1, 1, 0 },
/* L */{ 0, 0, 0, 1, 1, 1, 0, 0 },
/* M */{ 1, 1, 1, 0, 1, 1, 0, 0 },
/* N */{ 0, 0, 1, 0, 1, 0, 1, 0 },
/* O */{ 1, 1, 1, 1, 1, 1, 0, 0 },
/* P */{ 1, 1, 0, 0, 1, 1, 1, 0 },
/* Q */{ 1, 1, 1, 1, 0, 1, 1, 0 },
/* R */{ 1, 1, 0, 0, 1, 1, 1, 0 },
/* S */{ 1, 0, 1, 1, 0, 1, 1, 0 },
/* T */{ 0, 0, 0, 1, 1, 1, 1, 0 },
/* U */{ 0, 1, 1, 1, 1, 1, 0, 0 },
/* V */{ 0, 1, 1, 1, 1, 1, 0, 0 },
/* W */{ 0, 1, 1, 1, 1, 1, 0, 0 },
/* X */{ 0, 1, 1, 0, 1, 1, 1, 0 },
/* Y */{ 0, 1, 1, 1, 0, 1, 1, 0 },
/* Z */{ 1, 1, 0, 1, 1, 0, 1, 0 } };

const u8 SEG7_Numbers[10][8] = {
/* 0 */{ 1, 1, 1, 1, 1, 1, 0, 0 },
/* 1 */{ 0, 1, 1, 0, 0, 0, 0, 0 },
/* 2 */{ 1, 1, 0, 1, 1, 0, 1, 0 },
/* 3 */{ 1, 1, 1, 1, 0, 0, 1, 0 },
/* 4 */{ 0, 1, 1, 0, 0, 1, 1, 0 },
/* 5 */{ 1, 0, 1, 1, 0, 1, 1, 0 },
/* 6 */{ 1, 0, 1, 1, 1, 1, 1, 0 },
/* 7 */{ 1, 1, 1, 0, 0, 0, 0, 0 },
/* 8 */{ 1, 1, 1, 1, 1, 1, 1, 0 },
/* 9 */{ 1, 1, 1, 1, 0, 1, 1, 0 } };

void MSEG7_vDisplayInit(SEG7_Config_t* A_xSEG7) {
	GPIOx_PinConfig_t leg[8];
	//port assignment
	leg[SEG7_A].Port = A_xSEG7->SEG7_PORTA;
	leg[SEG7_B].Port = A_xSEG7->SEG7_PORTB;
	leg[SEG7_C].Port = A_xSEG7->SEG7_PORTC;
	leg[SEG7_D].Port = A_xSEG7->SEG7_PORTD;
	leg[SEG7_E].Port = A_xSEG7->SEG7_PORTE;
	leg[SEG7_F].Port = A_xSEG7->SEG7_PORTF;
	leg[SEG7_G].Port = A_xSEG7->SEG7_PORTG;
	leg[SEG7_DP].Port = A_xSEG7->SEG7_PORTDP;

	//pin assignment
	leg[SEG7_A].Pin = A_xSEG7->SEG7_PINA;
	leg[SEG7_B].Pin = A_xSEG7->SEG7_PINB;
	leg[SEG7_C].Pin = A_xSEG7->SEG7_PINC;
	leg[SEG7_D].Pin = A_xSEG7->SEG7_PIND;
	leg[SEG7_E].Pin = A_xSEG7->SEG7_PINE;
	leg[SEG7_F].Pin = A_xSEG7->SEG7_PINF;
	leg[SEG7_G].Pin = A_xSEG7->SEG7_PING;
	leg[SEG7_DP].Pin = A_xSEG7->SEG7_PINDP;

	//output setup & initialization
	for (int i = 0; i < 8; i++) {
		leg[i].Mode = GPIO_MODE_OUTPUT;
		leg[i].OutputType = GPIO_OT_PUSHPULL;
		leg[i].Speed = GPIO_SPEED_LOW;
		leg[i].PullType = GPIO_NO_PULL;

		MGPIO_vPinInit(&leg[i]);
	}
	A_xSEG7->intialized = SEG7_INTIALIZED;

}
;

void MSGE7_vDisplayOn(SEG7_Config_t* A_xSEG7) {
	if (A_xSEG7->intialized != SEG7_INTIALIZED) {
		return;
	}

	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTA, A_xSEG7->SEG7_PINA, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTB, A_xSEG7->SEG7_PINB, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTC, A_xSEG7->SEG7_PINC, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTD, A_xSEG7->SEG7_PIND, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTE, A_xSEG7->SEG7_PINE, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTF, A_xSEG7->SEG7_PINF, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTG, A_xSEG7->SEG7_PING, GPIO_HIGH);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTDP, A_xSEG7->SEG7_PINDP, GPIO_HIGH);

}

void MSGE7_vDisplayClear(SEG7_Config_t* A_xSEG7) {
	if (A_xSEG7->intialized != SEG7_INTIALIZED) {
		return;
	}

	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTA, A_xSEG7->SEG7_PINA, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTB, A_xSEG7->SEG7_PINB, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTC, A_xSEG7->SEG7_PINC, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTD, A_xSEG7->SEG7_PIND, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTE, A_xSEG7->SEG7_PINE, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTF, A_xSEG7->SEG7_PINF, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTG, A_xSEG7->SEG7_PING, GPIO_LOW);
	MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTDP, A_xSEG7->SEG7_PINDP, GPIO_LOW);
}

void MSGE7_vSetlegValue(SEG7_Config_t* A_xSEG7, u8 A_u8Leg, u8 A_u8LegValue) {
	if (A_xSEG7->intialized != SEG7_INTIALIZED) {
		return;
	}

	switch (A_u8Leg) {
	case SEG7_A:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTA, A_xSEG7->SEG7_PINA,
				A_u8LegValue);
		break;
	case SEG7_B:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTB, A_xSEG7->SEG7_PINB,
				A_u8LegValue);
		break;

	case SEG7_C:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTC, A_xSEG7->SEG7_PINC,
				A_u8LegValue);
		break;

	case SEG7_D:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTD, A_xSEG7->SEG7_PIND,
				A_u8LegValue);
		break;

	case SEG7_E:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTE, A_xSEG7->SEG7_PINE,
				A_u8LegValue);
		break;

	case SEG7_F:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTF, A_xSEG7->SEG7_PINF,
				A_u8LegValue);
		break;

	case SEG7_G:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTG, A_xSEG7->SEG7_PING,
				A_u8LegValue);
		break;

	case SEG7_DP:
		MGPIO_vSetPinValue(A_xSEG7->SEG7_PORTDP, A_xSEG7->SEG7_PINDP,
				A_u8LegValue);
		break;

	}

}

void MSGE7_vPrintChar(SEG7_Config_t* A_xSEG7, char A_chrPrint) {
	if (A_xSEG7->intialized != SEG7_INTIALIZED) {
		return;
	}

	MSGE7_vDisplayClear(A_xSEG7);

	if (A_chrPrint >= 'a' && A_chrPrint <= 'z') {

		int diff = A_chrPrint - 'a';

		if (diff <= 25)

		{
			for (int i = 0; i < 8; i++) {
				MSGE7_vSetlegValue(A_xSEG7, i,
						SEG7_Alphabet[diff][i]);
			}
		}

	}

	else if (A_chrPrint >= 'A' && A_chrPrint <= 'Z') {
		int diff = A_chrPrint - 'A';

		if (diff <= 25)

		{
			for (int i = 0; i < 8; i++) {
				MSGE7_vSetlegValue(A_xSEG7, i,
						SEG7_Alphabet[diff][i]);
			}
		}
	}

	else if (A_chrPrint >= '0' && A_chrPrint <= '9') {
		u8 diff = A_chrPrint - '0';

		if (diff <= 9)

		{
			for (int i = 0; i < 8; i++) {
				MSGE7_vSetlegValue(A_xSEG7, i,
						SEG7_Numbers[diff][i]);
			}
		}

	} else if (A_chrPrint == '.') {
		MSGE7_vSetlegValue(A_xSEG7, SEG7_DP, GPIO_HIGH);
	} else {
		//Unsupported character
	}

}

#endif /* MCAL_7_SEG_7_SEG_PRG_C_ */

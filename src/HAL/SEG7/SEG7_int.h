/*
 * 7_SEG_int.h
 *
 *  Created on: Aug 21, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_SEG7_SEG7_INT_H_
#define MCAL_SEG7_SEG7_INT_H_

#include "../../LIB/STD_TYPES.H"

// Legs
#define SEG7_A  0
#define SEG7_B  1
#define SEG7_C  2
#define SEG7_D  3
#define SEG7_E  4
#define SEG7_F  5
#define SEG7_G  6
#define SEG7_DP 7

#define SEG7_INTIALIZED 0xff

typedef struct {
	u8 SEG7_PORTA;
	u8 SEG7_PINA;

	u8 SEG7_PORTB;
	u8 SEG7_PINB;

	u8 SEG7_PORTC;
	u8 SEG7_PINC;

	u8 SEG7_PORTD;
	u8 SEG7_PIND;

	u8 SEG7_PORTE;
	u8 SEG7_PINE;

	u8 SEG7_PORTF;
	u8 SEG7_PINF;

	u8 SEG7_PORTG;
	u8 SEG7_PING;

	u8 SEG7_PORTDP;
	u8 SEG7_PINDP;

	u8 intialized;

} SEG7_Config_t;

void MSEG7_vDisplayInit(SEG7_Config_t* A_xSEG7);

void MSGE7_vDisplayOn(SEG7_Config_t* A_xSEG7);

void MSGE7_vDisplayClear(SEG7_Config_t* A_xSEG7);

void MSGE7_vSetlegValue(SEG7_Config_t* A_xSEG7, u8 A_u8Leg, u8 A_u8LegValue);

void MSGE7_vPrintChar(SEG7_Config_t* A_xSEG7, char A_chrPrint);

#endif /* MCAL_SEG7_SEG7_INT_H_ */

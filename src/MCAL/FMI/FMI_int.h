/*
 * FMI_int.h
 *
 *  Created on: Sep 9, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_FMI_FMI_INT_H_
#define MCAL_FMI_FMI_INT_H_

typedef enum {
	Sector0 = 0, Sector1, Sector2, Sector3, Sector4, Sector5
}Sector_t ;

void MFMI_vSectorErase(Sector_t A_xSectorNo);
void MFMI_vMassErase();
void MFMI_vProgramFlash(u32 A_u32Address, u16* A_u32PrData, u32 DataLength);

#endif /* MCAL_FMI_FMI_INT_H_ */

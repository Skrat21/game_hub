/*
 * SPI_int.h
 *
 *  Created on: Sep 2, 2026
 *      Author: AHMED SHERIF
 */

#ifndef MCAL_SPI_SPI_INT_H_
#define MCAL_SPI_SPI_INT_H_

#include "../../LIB/STD_TYPES.h"

void MSPI_vInit();
u8 MSPI_vTransceive(u8 A_u8Data);
void MSPI_vTransceiveBuffer(const u8 *A_u8Data, u32 A_u32Size);

#endif /* MCAL_SPI_SPI_INT_H_ */

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#include "FMI_int.h"
#include "FMI_prv.h"


static void check_and_unlock()
{
	if (GET_BIT(FMI->CR, LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}
}


void MFMI_vSectorErase(Sector_t A_xSectorNo)
{
	check_and_unlock();

	/* Clear SNB bits */
	FMI->CR &= ~(0b1111 << SNB);

	/* Select sector */
	FMI->CR |= (A_xSectorNo << SNB);

	/* Sector erase */
	SET_BIT(FMI->CR, SER);

	/* Start erase */
	SET_BIT(FMI->CR, STRT);

	while (GET_BIT(FMI->SR, BSY))
		;

	/* Clear sector erase */
	CLR_BIT(FMI->CR, SER);
}


void MFMI_vMassErase()
{
	while (GET_BIT(FMI->SR, BSY))
		;

	check_and_unlock();

	SET_BIT(FMI->CR, MER);
	SET_BIT(FMI->CR, STRT);

	while (GET_BIT(FMI->SR, BSY))
		;

	CLR_BIT(FMI->CR, MER);
}


void MFMI_vProgramFlash(u32 A_u32Address, u16* A_u32PrData, u32 DataLength)
{
	while (GET_BIT(FMI->SR, BSY))
		;

	check_and_unlock();

	/* PSIZE = 01 --> Half-word (16-bit) */
	FMI->CR &= ~(0b11 << PSIZE);
	FMI->CR |= (0b01 << PSIZE);

	SET_BIT(FMI->CR, PG);

	for(u16 i = 0; i < DataLength; i++)
	{
		*(volatile u16*)A_u32Address = A_u32PrData[i];

		A_u32Address += 2;

		while(GET_BIT(FMI->SR, BSY))
			;
	}

	CLR_BIT(FMI->CR, PG);
}

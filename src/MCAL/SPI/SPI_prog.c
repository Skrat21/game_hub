
#include "SPI_int.h"
#include "SPI_priv.h"

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#define SPI_TRANSFER_TIMEOUT 1000000UL

void MSPI_vInit() {

	// SW slave
	SET_BIT(SPI1->CR1, SSM);

	// Master
	SET_BIT(SPI1->CR1, SSI);

	// data
	CLR_BIT(SPI1->CR1, DFF);

	// MSB
	CLR_BIT(SPI1->CR1, LSBFIRST);

	// SELECT MASTER
	SET_BIT(SPI1->CR1,MSTR);

	// POLARITY
	CLR_BIT(SPI1->CR1,CPOL);

	// PHASE
	CLR_BIT(SPI1->CR1,CPHA);

	// EMABLE SPI
	SET_BIT(SPI1->CR1,SPE);
}

u8 MSPI_vTransceive(u8 A_u8Data) {
	volatile u32 timeout = SPI_TRANSFER_TIMEOUT;
	while (!GET_BIT(SPI1->SR,TXE) && timeout > 0) {
		timeout--;
	}
	if (timeout == 0) {
		return 0;
	}
	SPI1->DR = A_u8Data;

	timeout = SPI_TRANSFER_TIMEOUT;
	while(!GET_BIT(SPI1->SR,RXNE) && timeout > 0) {
		timeout--;
	}
	if (timeout == 0) {
		return 0;
	}
	return SPI1->DR;
}


void MSPI_vTransceiveBuffer(const u8 *A_u8Data, u32 A_u32Size)
{
    for (u32 i = 0; i < A_u32Size; i++)
    {
		volatile u32 timeout = SPI_TRANSFER_TIMEOUT;
		while (!GET_BIT(SPI1->SR, TXE) && timeout > 0) {
			timeout--;
		}
		if (timeout == 0) {
			return;
		}

        SPI1->DR = A_u8Data[i];

		timeout = SPI_TRANSFER_TIMEOUT;
		while (!GET_BIT(SPI1->SR, RXNE) && timeout > 0) {
			timeout--;
		}
		if (timeout == 0) {
			return;
		}

        (void)SPI1->DR;
    }
}


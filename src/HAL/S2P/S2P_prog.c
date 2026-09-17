
#include "S2P_cfg.h"
#include "S2P_int.h"

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

void HS2P_vInit(S2P_Init_t* A)
{
	GPIOx_PinConfig_t data = {
			.Port = A->Dataport,
			.Pin = A->Datapin,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputType = GPIO_OT_PUSHPULL,
			.Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL
	};

	GPIOx_PinConfig_t latch = {
				.Port = A->LatchCLKPort,
				.Pin = A->LatchCLKPin,
				.Mode = GPIO_MODE_OUTPUT,
				.OutputType = GPIO_OT_PUSHPULL,
				.Speed = GPIO_SPEED_LOW,
				.PullType = GPIO_NO_PULL
		};

	GPIOx_PinConfig_t shift = {
				.Port = A->ShiftCLKPort,
				.Pin = A->ShiftCLKPin,
				.Mode = GPIO_MODE_OUTPUT,
				.OutputType = GPIO_OT_PUSHPULL,
				.Speed = GPIO_SPEED_LOW,
				.PullType = GPIO_NO_PULL
		};

	MGPIO_vPinInit(&data);
	MGPIO_vPinInit(&latch);
	MGPIO_vPinInit(&shift);

}

static void HS2P_vShiftData(S2P_Init_t * A_xInit)
{
	//low to high enables shift pin
	MGPIO_vSetPinValue(A_xInit->ShiftCLKPort,A_xInit->ShiftCLKPin,GPIO_HIGH);
//	MSYSTICK_vSetDelay_ms(1); //small delay
	MGPIO_vSetPinValue(A_xInit->ShiftCLKPort,A_xInit->ShiftCLKPin,GPIO_LOW);

}

static void HS2P_vLatchData(S2P_Init_t * A_xInit)
{
	//low to high enables latch pin
	MGPIO_vSetPinValue(A_xInit->LatchCLKPort,A_xInit->LatchCLKPin,GPIO_HIGH);
//	MSYSTICK_vSetDelay_ms(1); //small delay
	MGPIO_vSetPinValue(A_xInit->LatchCLKPort,A_xInit->LatchCLKPin,GPIO_LOW);

}


void HS2P_vSendData(S2P_Init_t * A_xInit, u32 A_u32Byte)
{
    for(int i = (8*S2P_NO_OF_SHIFT_REG) - 1; i >= 0; i--)
    {
        MGPIO_vSetPinValue(A_xInit->Dataport, A_xInit->Datapin, GET_BIT(A_u32Byte,i));
        HS2P_vShiftData(A_xInit);
    }
    HS2P_vLatchData(A_xInit);
}

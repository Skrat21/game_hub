################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/APP/Common/BOARD_ANIM.c \
../src/APP/Common/FONT3x5.c \
../src/APP/Common/LOADING_ANIM.c 

OBJS += \
./src/APP/Common/BOARD_ANIM.o \
./src/APP/Common/FONT3x5.o \
./src/APP/Common/LOADING_ANIM.o 

C_DEPS += \
./src/APP/Common/BOARD_ANIM.d \
./src/APP/Common/FONT3x5.d \
./src/APP/Common/LOADING_ANIM.d 


# Each subdirectory must supply rules for building sources it contributes
src/APP/Common/%.o: ../src/APP/Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -O0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



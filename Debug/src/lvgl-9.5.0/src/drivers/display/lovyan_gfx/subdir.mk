################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/lvgl-9.5.0/src/drivers/display/lovyan_gfx/lv_lovyan_gfx.cpp 

OBJS += \
./src/lvgl-9.5.0/src/drivers/display/lovyan_gfx/lv_lovyan_gfx.o 

CPP_DEPS += \
./src/lvgl-9.5.0/src/drivers/display/lovyan_gfx/lv_lovyan_gfx.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/drivers/display/lovyan_gfx/%.o: ../src/lvgl-9.5.0/src/drivers/display/lovyan_gfx/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



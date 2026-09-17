################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/stdlib/builtin/lv_mem_core_builtin.c \
../src/lvgl-9.5.0/src/stdlib/builtin/lv_sprintf_builtin.c \
../src/lvgl-9.5.0/src/stdlib/builtin/lv_string_builtin.c \
../src/lvgl-9.5.0/src/stdlib/builtin/lv_tlsf.c 

OBJS += \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_mem_core_builtin.o \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_sprintf_builtin.o \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_string_builtin.o \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_tlsf.o 

C_DEPS += \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_mem_core_builtin.d \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_sprintf_builtin.d \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_string_builtin.d \
./src/lvgl-9.5.0/src/stdlib/builtin/lv_tlsf.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/stdlib/builtin/%.o: ../src/lvgl-9.5.0/src/stdlib/builtin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



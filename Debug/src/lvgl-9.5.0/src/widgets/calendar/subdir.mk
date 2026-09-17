################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/widgets/calendar/lv_calendar.c \
../src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_chinese.c \
../src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_arrow.c \
../src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_dropdown.c 

OBJS += \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar.o \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_chinese.o \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_arrow.o \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_dropdown.o 

C_DEPS += \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar.d \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_chinese.d \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_arrow.d \
./src/lvgl-9.5.0/src/widgets/calendar/lv_calendar_header_dropdown.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/widgets/calendar/%.o: ../src/lvgl-9.5.0/src/widgets/calendar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/env_support/pikascript/pika_lv_point_t.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lv_timer_t.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lv_wegit.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lvgl.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_indev_t.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_event.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_obj.c \
../src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_style_t.c 

OBJS += \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_point_t.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_timer_t.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_wegit.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_indev_t.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_event.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_obj.o \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_style_t.o 

C_DEPS += \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_point_t.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_timer_t.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lv_wegit.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_indev_t.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_event.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_obj.d \
./src/lvgl-9.5.0/env_support/pikascript/pika_lvgl_lv_style_t.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/env_support/pikascript/%.o: ../src/lvgl-9.5.0/env_support/pikascript/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



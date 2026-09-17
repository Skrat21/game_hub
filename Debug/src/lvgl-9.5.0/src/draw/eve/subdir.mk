################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_arc.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_fill.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_image.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_letter.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_line.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_ram_g.c \
../src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_triangle.c \
../src/lvgl-9.5.0/src/draw/eve/lv_eve.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_arc.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_fill.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_image.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_letter.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_line.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_ram_g.o \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_triangle.o \
./src/lvgl-9.5.0/src/draw/eve/lv_eve.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_arc.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_fill.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_image.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_letter.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_line.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_ram_g.d \
./src/lvgl-9.5.0/src/draw/eve/lv_draw_eve_triangle.d \
./src/lvgl-9.5.0/src/draw/eve/lv_eve.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/eve/%.o: ../src/lvgl-9.5.0/src/draw/eve/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



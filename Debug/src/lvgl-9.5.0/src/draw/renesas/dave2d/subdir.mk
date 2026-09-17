################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_arc.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_border.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_fill.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_image.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_label.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_line.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_mask_rectangle.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_triangle.c \
../src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_utils.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_arc.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_border.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_fill.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_image.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_label.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_line.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_mask_rectangle.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_triangle.o \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_utils.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_arc.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_border.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_fill.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_image.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_label.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_line.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_mask_rectangle.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_triangle.d \
./src/lvgl-9.5.0/src/draw/renesas/dave2d/lv_draw_dave2d_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/renesas/dave2d/%.o: ../src/lvgl-9.5.0/src/draw/renesas/dave2d/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



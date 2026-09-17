################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_arc.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_blur.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_border.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_box_shadow.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_fill.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_grad.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_img.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_letter.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_line.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask_rect.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_transform.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_triangle.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_utils.c \
../src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_vector.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_arc.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_blur.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_border.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_box_shadow.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_fill.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_grad.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_img.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_letter.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_line.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask_rect.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_transform.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_triangle.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_utils.o \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_vector.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_arc.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_blur.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_border.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_box_shadow.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_fill.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_grad.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_img.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_letter.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_line.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_mask_rect.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_transform.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_triangle.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_utils.d \
./src/lvgl-9.5.0/src/draw/sw/lv_draw_sw_vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/sw/%.o: ../src/lvgl-9.5.0/src/draw/sw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



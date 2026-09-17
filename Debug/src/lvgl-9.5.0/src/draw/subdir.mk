################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/lv_draw.c \
../src/lvgl-9.5.0/src/draw/lv_draw_3d.c \
../src/lvgl-9.5.0/src/draw/lv_draw_arc.c \
../src/lvgl-9.5.0/src/draw/lv_draw_blur.c \
../src/lvgl-9.5.0/src/draw/lv_draw_buf.c \
../src/lvgl-9.5.0/src/draw/lv_draw_image.c \
../src/lvgl-9.5.0/src/draw/lv_draw_label.c \
../src/lvgl-9.5.0/src/draw/lv_draw_line.c \
../src/lvgl-9.5.0/src/draw/lv_draw_mask.c \
../src/lvgl-9.5.0/src/draw/lv_draw_rect.c \
../src/lvgl-9.5.0/src/draw/lv_draw_triangle.c \
../src/lvgl-9.5.0/src/draw/lv_draw_vector.c \
../src/lvgl-9.5.0/src/draw/lv_image_decoder.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/lv_draw.o \
./src/lvgl-9.5.0/src/draw/lv_draw_3d.o \
./src/lvgl-9.5.0/src/draw/lv_draw_arc.o \
./src/lvgl-9.5.0/src/draw/lv_draw_blur.o \
./src/lvgl-9.5.0/src/draw/lv_draw_buf.o \
./src/lvgl-9.5.0/src/draw/lv_draw_image.o \
./src/lvgl-9.5.0/src/draw/lv_draw_label.o \
./src/lvgl-9.5.0/src/draw/lv_draw_line.o \
./src/lvgl-9.5.0/src/draw/lv_draw_mask.o \
./src/lvgl-9.5.0/src/draw/lv_draw_rect.o \
./src/lvgl-9.5.0/src/draw/lv_draw_triangle.o \
./src/lvgl-9.5.0/src/draw/lv_draw_vector.o \
./src/lvgl-9.5.0/src/draw/lv_image_decoder.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/lv_draw.d \
./src/lvgl-9.5.0/src/draw/lv_draw_3d.d \
./src/lvgl-9.5.0/src/draw/lv_draw_arc.d \
./src/lvgl-9.5.0/src/draw/lv_draw_blur.d \
./src/lvgl-9.5.0/src/draw/lv_draw_buf.d \
./src/lvgl-9.5.0/src/draw/lv_draw_image.d \
./src/lvgl-9.5.0/src/draw/lv_draw_label.d \
./src/lvgl-9.5.0/src/draw/lv_draw_line.d \
./src/lvgl-9.5.0/src/draw/lv_draw_mask.d \
./src/lvgl-9.5.0/src/draw/lv_draw_rect.d \
./src/lvgl-9.5.0/src/draw/lv_draw_triangle.d \
./src/lvgl-9.5.0/src/draw/lv_draw_vector.d \
./src/lvgl-9.5.0/src/draw/lv_image_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/%.o: ../src/lvgl-9.5.0/src/draw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



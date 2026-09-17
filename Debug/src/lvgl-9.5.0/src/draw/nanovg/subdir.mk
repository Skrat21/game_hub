################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_3d.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_arc.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_border.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_box_shadow.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_fill.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_grad.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_image.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_label.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_layer.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_line.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_mask_rect.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_triangle.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_vector.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_fbo_cache.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_image_cache.c \
../src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_utils.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_3d.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_arc.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_border.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_box_shadow.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_fill.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_grad.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_image.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_label.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_layer.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_line.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_mask_rect.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_triangle.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_vector.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_fbo_cache.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_image_cache.o \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_utils.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_3d.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_arc.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_border.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_box_shadow.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_fill.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_grad.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_image.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_label.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_layer.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_line.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_mask_rect.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_triangle.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_draw_nanovg_vector.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_fbo_cache.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_image_cache.d \
./src/lvgl-9.5.0/src/draw/nanovg/lv_nanovg_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/nanovg/%.o: ../src/lvgl-9.5.0/src/draw/nanovg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



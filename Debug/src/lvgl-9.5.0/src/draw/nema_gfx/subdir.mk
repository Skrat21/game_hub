################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_arc.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_border.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_fill.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_img.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_label.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_layer.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_line.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_utils.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_vector.c \
../src/lvgl-9.5.0/src/draw/nema_gfx/lv_nema_gfx_path.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_arc.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_border.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_fill.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_img.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_label.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_layer.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_line.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_utils.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_vector.o \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_nema_gfx_path.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_arc.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_border.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_fill.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_img.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_label.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_layer.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_line.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_utils.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_draw_nema_gfx_vector.d \
./src/lvgl-9.5.0/src/draw/nema_gfx/lv_nema_gfx_path.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/nema_gfx/%.o: ../src/lvgl-9.5.0/src/draw/nema_gfx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



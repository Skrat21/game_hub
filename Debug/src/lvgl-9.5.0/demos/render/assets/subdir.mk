################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/demos/render/assets/img_render_arc_bg.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888_premultiplied.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_i1.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_l8.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565_swapped.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565a8.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb888.c \
../src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_xrgb8888.c 

OBJS += \
./src/lvgl-9.5.0/demos/render/assets/img_render_arc_bg.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888_premultiplied.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_i1.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_l8.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565_swapped.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565a8.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb888.o \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_xrgb8888.o 

C_DEPS += \
./src/lvgl-9.5.0/demos/render/assets/img_render_arc_bg.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_argb8888_premultiplied.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_i1.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_l8.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565_swapped.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb565a8.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_rgb888.d \
./src/lvgl-9.5.0/demos/render/assets/img_render_lvgl_logo_xrgb8888.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/demos/render/assets/%.o: ../src/lvgl-9.5.0/demos/render/assets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



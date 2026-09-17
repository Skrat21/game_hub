################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/examples/assets/animimg001.c \
../src/lvgl-9.5.0/examples/assets/animimg002.c \
../src/lvgl-9.5.0/examples/assets/animimg003.c \
../src/lvgl-9.5.0/examples/assets/img_caret_down.c \
../src/lvgl-9.5.0/examples/assets/img_cogwheel_argb.c \
../src/lvgl-9.5.0/examples/assets/img_cogwheel_indexed16.c \
../src/lvgl-9.5.0/examples/assets/img_cogwheel_rgb.c \
../src/lvgl-9.5.0/examples/assets/img_hand.c \
../src/lvgl-9.5.0/examples/assets/img_multilang_like.c \
../src/lvgl-9.5.0/examples/assets/img_skew_strip.c \
../src/lvgl-9.5.0/examples/assets/img_star.c \
../src/lvgl-9.5.0/examples/assets/img_svg_img.c \
../src/lvgl-9.5.0/examples/assets/img_transform_avatar_15.c \
../src/lvgl-9.5.0/examples/assets/imgbtn_left.c \
../src/lvgl-9.5.0/examples/assets/imgbtn_mid.c \
../src/lvgl-9.5.0/examples/assets/imgbtn_right.c 

OBJS += \
./src/lvgl-9.5.0/examples/assets/animimg001.o \
./src/lvgl-9.5.0/examples/assets/animimg002.o \
./src/lvgl-9.5.0/examples/assets/animimg003.o \
./src/lvgl-9.5.0/examples/assets/img_caret_down.o \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_argb.o \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_indexed16.o \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_rgb.o \
./src/lvgl-9.5.0/examples/assets/img_hand.o \
./src/lvgl-9.5.0/examples/assets/img_multilang_like.o \
./src/lvgl-9.5.0/examples/assets/img_skew_strip.o \
./src/lvgl-9.5.0/examples/assets/img_star.o \
./src/lvgl-9.5.0/examples/assets/img_svg_img.o \
./src/lvgl-9.5.0/examples/assets/img_transform_avatar_15.o \
./src/lvgl-9.5.0/examples/assets/imgbtn_left.o \
./src/lvgl-9.5.0/examples/assets/imgbtn_mid.o \
./src/lvgl-9.5.0/examples/assets/imgbtn_right.o 

C_DEPS += \
./src/lvgl-9.5.0/examples/assets/animimg001.d \
./src/lvgl-9.5.0/examples/assets/animimg002.d \
./src/lvgl-9.5.0/examples/assets/animimg003.d \
./src/lvgl-9.5.0/examples/assets/img_caret_down.d \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_argb.d \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_indexed16.d \
./src/lvgl-9.5.0/examples/assets/img_cogwheel_rgb.d \
./src/lvgl-9.5.0/examples/assets/img_hand.d \
./src/lvgl-9.5.0/examples/assets/img_multilang_like.d \
./src/lvgl-9.5.0/examples/assets/img_skew_strip.d \
./src/lvgl-9.5.0/examples/assets/img_star.d \
./src/lvgl-9.5.0/examples/assets/img_svg_img.d \
./src/lvgl-9.5.0/examples/assets/img_transform_avatar_15.d \
./src/lvgl-9.5.0/examples/assets/imgbtn_left.d \
./src/lvgl-9.5.0/examples/assets/imgbtn_mid.d \
./src/lvgl-9.5.0/examples/assets/imgbtn_right.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/examples/assets/%.o: ../src/lvgl-9.5.0/examples/assets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



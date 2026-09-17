################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_barcode.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_bin_decoder.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_bmp.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_ffmpeg.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_font_stress.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_freetype.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_gif.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_libjpeg_turbo.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_libpng.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_libwebp.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_lodepng.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_memmove.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_qrcode.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_svg_decoder.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_tiny_ttf.c \
../src/lvgl-9.5.0/tests/src/test_cases/libs/test_tjpgd.c 

OBJS += \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_barcode.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_bin_decoder.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_bmp.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_ffmpeg.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_font_stress.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_freetype.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_gif.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libjpeg_turbo.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libpng.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libwebp.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_lodepng.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_memmove.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_qrcode.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_svg_decoder.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_tiny_ttf.o \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_tjpgd.o 

C_DEPS += \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_barcode.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_bin_decoder.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_bmp.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_ffmpeg.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_font_stress.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_freetype.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_gif.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libjpeg_turbo.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libpng.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_libwebp.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_lodepng.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_memmove.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_qrcode.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_svg_decoder.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_tiny_ttf.d \
./src/lvgl-9.5.0/tests/src/test_cases/libs/test_tjpgd.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/tests/src/test_cases/libs/%.o: ../src/lvgl-9.5.0/tests/src/test_cases/libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



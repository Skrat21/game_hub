################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_avatar.c \
../src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.c \
../src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.c \
../src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.c 

OBJS += \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_avatar.o \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.o \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.o \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.o 

C_DEPS += \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_avatar.d \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.d \
./src/lvgl-9.5.0/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.d \
./src/lvgl-9.5.0/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/demos/benchmark/assets/%.o: ../src/lvgl-9.5.0/demos/benchmark/assets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



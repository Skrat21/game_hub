################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/debugging/test/lv_test_display.c \
../src/lvgl-9.5.0/src/debugging/test/lv_test_fs.c \
../src/lvgl-9.5.0/src/debugging/test/lv_test_helpers.c \
../src/lvgl-9.5.0/src/debugging/test/lv_test_indev.c \
../src/lvgl-9.5.0/src/debugging/test/lv_test_indev_gesture.c \
../src/lvgl-9.5.0/src/debugging/test/lv_test_screenshot_compare.c 

OBJS += \
./src/lvgl-9.5.0/src/debugging/test/lv_test_display.o \
./src/lvgl-9.5.0/src/debugging/test/lv_test_fs.o \
./src/lvgl-9.5.0/src/debugging/test/lv_test_helpers.o \
./src/lvgl-9.5.0/src/debugging/test/lv_test_indev.o \
./src/lvgl-9.5.0/src/debugging/test/lv_test_indev_gesture.o \
./src/lvgl-9.5.0/src/debugging/test/lv_test_screenshot_compare.o 

C_DEPS += \
./src/lvgl-9.5.0/src/debugging/test/lv_test_display.d \
./src/lvgl-9.5.0/src/debugging/test/lv_test_fs.d \
./src/lvgl-9.5.0/src/debugging/test/lv_test_helpers.d \
./src/lvgl-9.5.0/src/debugging/test/lv_test_indev.d \
./src/lvgl-9.5.0/src/debugging/test/lv_test_indev_gesture.d \
./src/lvgl-9.5.0/src/debugging/test/lv_test_screenshot_compare.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/debugging/test/%.o: ../src/lvgl-9.5.0/src/debugging/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



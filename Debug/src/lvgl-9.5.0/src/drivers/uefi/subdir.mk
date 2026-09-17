################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_context.c \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_display.c \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_keyboard.c \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_pointer.c \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_touch.c \
../src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_private.c 

OBJS += \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_context.o \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_display.o \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_keyboard.o \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_pointer.o \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_touch.o \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_private.o 

C_DEPS += \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_context.d \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_display.d \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_keyboard.d \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_pointer.d \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_indev_touch.d \
./src/lvgl-9.5.0/src/drivers/uefi/lv_uefi_private.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/drivers/uefi/%.o: ../src/lvgl-9.5.0/src/drivers/uefi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



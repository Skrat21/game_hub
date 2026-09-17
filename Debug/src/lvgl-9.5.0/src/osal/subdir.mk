################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/osal/lv_cmsis_rtos2.c \
../src/lvgl-9.5.0/src/osal/lv_freertos.c \
../src/lvgl-9.5.0/src/osal/lv_linux.c \
../src/lvgl-9.5.0/src/osal/lv_mqx.c \
../src/lvgl-9.5.0/src/osal/lv_os.c \
../src/lvgl-9.5.0/src/osal/lv_os_none.c \
../src/lvgl-9.5.0/src/osal/lv_pthread.c \
../src/lvgl-9.5.0/src/osal/lv_rtthread.c \
../src/lvgl-9.5.0/src/osal/lv_sdl2.c \
../src/lvgl-9.5.0/src/osal/lv_windows.c 

OBJS += \
./src/lvgl-9.5.0/src/osal/lv_cmsis_rtos2.o \
./src/lvgl-9.5.0/src/osal/lv_freertos.o \
./src/lvgl-9.5.0/src/osal/lv_linux.o \
./src/lvgl-9.5.0/src/osal/lv_mqx.o \
./src/lvgl-9.5.0/src/osal/lv_os.o \
./src/lvgl-9.5.0/src/osal/lv_os_none.o \
./src/lvgl-9.5.0/src/osal/lv_pthread.o \
./src/lvgl-9.5.0/src/osal/lv_rtthread.o \
./src/lvgl-9.5.0/src/osal/lv_sdl2.o \
./src/lvgl-9.5.0/src/osal/lv_windows.o 

C_DEPS += \
./src/lvgl-9.5.0/src/osal/lv_cmsis_rtos2.d \
./src/lvgl-9.5.0/src/osal/lv_freertos.d \
./src/lvgl-9.5.0/src/osal/lv_linux.d \
./src/lvgl-9.5.0/src/osal/lv_mqx.d \
./src/lvgl-9.5.0/src/osal/lv_os.d \
./src/lvgl-9.5.0/src/osal/lv_os_none.d \
./src/lvgl-9.5.0/src/osal/lv_pthread.d \
./src/lvgl-9.5.0/src/osal/lv_rtthread.d \
./src/lvgl-9.5.0/src/osal/lv_sdl2.d \
./src/lvgl-9.5.0/src/osal/lv_windows.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/osal/%.o: ../src/lvgl-9.5.0/src/osal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



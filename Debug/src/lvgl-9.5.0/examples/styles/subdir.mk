################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/examples/styles/lv_example_style_1.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_10.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_11.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_12.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_13.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_14.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_15.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_16.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_17.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_18.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_19.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_2.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_20.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_21.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_3.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_4.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_5.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_6.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_7.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_8.c \
../src/lvgl-9.5.0/examples/styles/lv_example_style_9.c 

OBJS += \
./src/lvgl-9.5.0/examples/styles/lv_example_style_1.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_10.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_11.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_12.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_13.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_14.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_15.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_16.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_17.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_18.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_19.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_2.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_20.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_21.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_3.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_4.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_5.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_6.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_7.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_8.o \
./src/lvgl-9.5.0/examples/styles/lv_example_style_9.o 

C_DEPS += \
./src/lvgl-9.5.0/examples/styles/lv_example_style_1.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_10.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_11.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_12.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_13.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_14.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_15.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_16.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_17.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_18.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_19.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_2.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_20.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_21.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_3.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_4.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_5.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_6.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_7.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_8.d \
./src/lvgl-9.5.0/examples/styles/lv_example_style_9.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/examples/styles/%.o: ../src/lvgl-9.5.0/examples/styles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



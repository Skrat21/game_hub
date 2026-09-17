################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/demos/widgets/lv_demo_widgets.c \
../src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_analytics.c \
../src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_components.c \
../src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_profile.c \
../src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_shop.c 

OBJS += \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets.o \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_analytics.o \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_components.o \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_profile.o \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_shop.o 

C_DEPS += \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets.d \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_analytics.d \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_components.d \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_profile.d \
./src/lvgl-9.5.0/demos/widgets/lv_demo_widgets_shop.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/demos/widgets/%.o: ../src/lvgl-9.5.0/demos/widgets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



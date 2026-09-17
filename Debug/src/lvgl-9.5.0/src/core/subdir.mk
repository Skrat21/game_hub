################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/core/lv_group.c \
../src/lvgl-9.5.0/src/core/lv_obj.c \
../src/lvgl-9.5.0/src/core/lv_obj_class.c \
../src/lvgl-9.5.0/src/core/lv_obj_draw.c \
../src/lvgl-9.5.0/src/core/lv_obj_event.c \
../src/lvgl-9.5.0/src/core/lv_obj_id_builtin.c \
../src/lvgl-9.5.0/src/core/lv_obj_pos.c \
../src/lvgl-9.5.0/src/core/lv_obj_property.c \
../src/lvgl-9.5.0/src/core/lv_obj_scroll.c \
../src/lvgl-9.5.0/src/core/lv_obj_style.c \
../src/lvgl-9.5.0/src/core/lv_obj_style_gen.c \
../src/lvgl-9.5.0/src/core/lv_obj_tree.c \
../src/lvgl-9.5.0/src/core/lv_observer.c \
../src/lvgl-9.5.0/src/core/lv_refr.c 

OBJS += \
./src/lvgl-9.5.0/src/core/lv_group.o \
./src/lvgl-9.5.0/src/core/lv_obj.o \
./src/lvgl-9.5.0/src/core/lv_obj_class.o \
./src/lvgl-9.5.0/src/core/lv_obj_draw.o \
./src/lvgl-9.5.0/src/core/lv_obj_event.o \
./src/lvgl-9.5.0/src/core/lv_obj_id_builtin.o \
./src/lvgl-9.5.0/src/core/lv_obj_pos.o \
./src/lvgl-9.5.0/src/core/lv_obj_property.o \
./src/lvgl-9.5.0/src/core/lv_obj_scroll.o \
./src/lvgl-9.5.0/src/core/lv_obj_style.o \
./src/lvgl-9.5.0/src/core/lv_obj_style_gen.o \
./src/lvgl-9.5.0/src/core/lv_obj_tree.o \
./src/lvgl-9.5.0/src/core/lv_observer.o \
./src/lvgl-9.5.0/src/core/lv_refr.o 

C_DEPS += \
./src/lvgl-9.5.0/src/core/lv_group.d \
./src/lvgl-9.5.0/src/core/lv_obj.d \
./src/lvgl-9.5.0/src/core/lv_obj_class.d \
./src/lvgl-9.5.0/src/core/lv_obj_draw.d \
./src/lvgl-9.5.0/src/core/lv_obj_event.d \
./src/lvgl-9.5.0/src/core/lv_obj_id_builtin.d \
./src/lvgl-9.5.0/src/core/lv_obj_pos.d \
./src/lvgl-9.5.0/src/core/lv_obj_property.d \
./src/lvgl-9.5.0/src/core/lv_obj_scroll.d \
./src/lvgl-9.5.0/src/core/lv_obj_style.d \
./src/lvgl-9.5.0/src/core/lv_obj_style_gen.d \
./src/lvgl-9.5.0/src/core/lv_obj_tree.d \
./src/lvgl-9.5.0/src/core/lv_observer.d \
./src/lvgl-9.5.0/src/core/lv_refr.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/core/%.o: ../src/lvgl-9.5.0/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



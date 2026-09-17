################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_buf_pxp.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_fill.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_img.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_layer.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_cfg.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_osa.c \
../src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_utils.c 

OBJS += \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_buf_pxp.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_fill.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_img.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_layer.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_cfg.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_osa.o \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_utils.o 

C_DEPS += \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_buf_pxp.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_fill.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_img.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_draw_pxp_layer.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_cfg.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_osa.d \
./src/lvgl-9.5.0/src/draw/nxp/pxp/lv_pxp_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/draw/nxp/pxp/%.o: ../src/lvgl-9.5.0/src/draw/nxp/pxp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



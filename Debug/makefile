################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/stm32f4-hal/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/OS/subdir.mk
-include src/MCAL/USART/subdir.mk
-include src/MCAL/SYSTICK/subdir.mk
-include src/MCAL/SYSCFG/subdir.mk
-include src/MCAL/SPI/subdir.mk
-include src/MCAL/RCC/subdir.mk
-include src/MCAL/NVIC/subdir.mk
-include src/MCAL/GPIO/subdir.mk
-include src/MCAL/FMI/subdir.mk
-include src/MCAL/EXTI/subdir.mk
-include src/HAL/TFT/subdir.mk
-include src/HAL/SEG7/subdir.mk
-include src/HAL/S2P/subdir.mk
-include src/HAL/OTA_LED/subdir.mk
-include src/HAL/LEDMATRIX/subdir.mk
-include src/HAL/IR/subdir.mk
-include src/HAL/ESP8266/subdir.mk
-include src/HAL/DAC/subdir.mk
-include src/APP/Yatzy/Animation/subdir.mk
-include src/APP/Yatzy/subdir.mk
-include src/APP/TicTacToe/Animation/subdir.mk
-include src/APP/TicTacToe/subdir.mk
-include src/APP/SnakeAndLadder/Animation/subdir.mk
-include src/APP/SnakeAndLadder/subdir.mk
-include src/APP/RockPaperScissors/Animation/subdir.mk
-include src/APP/RockPaperScissors/subdir.mk
-include src/APP/Offline/subdir.mk
-include src/APP/OTA/subdir.mk
-include src/APP/Menu/subdir.mk
-include src/APP/Memory/Animation/subdir.mk
-include src/APP/Memory/subdir.mk
-include src/APP/MathQuiz/Animation/subdir.mk
-include src/APP/MathQuiz/subdir.mk
-include src/APP/HEXPARSER/subdir.mk
-include src/APP/Connect4/Animation/subdir.mk
-include src/APP/Connect4/subdir.mk
-include src/APP/Common/subdir.mk
-include src/APP/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
fixed\ project.hex \

SECONDARY_SIZE += \
fixed\ project.siz \


# All Target
all: fixed\ project.elf secondary-outputs

# Tool invocations
fixed\ project.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T mem.ld -T libs.ld -T sections.ld -T appmeta.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"fixed project.map" --specs=nano.specs -o "fixed project.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

fixed\ project.hex: fixed\ project.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "fixed project.elf"  "fixed project.hex"
	@echo 'Finished building: $@'
	@echo ' '

fixed\ project.siz: fixed\ project.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "fixed project.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) "fixed project.elf"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

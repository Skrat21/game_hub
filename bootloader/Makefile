################################################################################
# Bootloader image build.
#
# The bootloader owns Sectors 0-1 (0x08000000-0x08007FFF) and performs the OTA
# download/flash, so it is built and linked separately from the application
# (see Debug/makefile, which links the application at 0x08008000).
#
# Usage:  mingw32-make -C bootloader all
################################################################################

PROJ     := ..
LDDIR    := $(PROJ)/ldscripts

CC       := arm-none-eabi-gcc
LD       := arm-none-eabi-gcc
OBJCOPY  := arm-none-eabi-objcopy
SIZE     := arm-none-eabi-size

TARGET   := bootloader

ifeq ($(OS),Windows_NT)
RM := powershell -NoProfile -ExecutionPolicy Bypass -File $(PROJ)/tools/rm.ps1
else
RM := rm -rf
endif

CFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
  -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections \
  -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3 \
  -DDEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 \
  -I"$(PROJ)/include" -I"$(PROJ)/system/include" \
  -I"$(PROJ)/system/include/cmsis" -I"$(PROJ)/system/include/stm32f4-hal" \
  -std=gnu11 -O0

LDFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og \
  -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -g3 \
  -T "$(LDDIR)/bootloader_mem.ld" -T "$(LDDIR)/libs.ld" -T "$(LDDIR)/sections.ld" \
  -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(TARGET).map" \
  --specs=nano.specs

BOOT_SRCS := \
  src/BOOTLOADER/BOOTLOADER_prog.c \
  system/src/newlib/_startup.c \
  system/src/newlib/_exit.c \
  system/src/newlib/_sbrk.c \
  system/src/newlib/_syscalls.c \
  system/src/cortexm/_initialize_hardware.c \
  system/src/cortexm/_reset_hardware.c \
  system/src/cortexm/exception_handlers.c \
  system/src/cmsis/system_stm32f4xx.c \
  system/src/cmsis/vectors_stm32f401xc.c

vpath %.c $(PROJ)/src/BOOTLOADER:$(PROJ)/system/src/newlib:$(PROJ)/system/src/cortexm:$(PROJ)/system/src/cmsis

BOBJS := $(addprefix b_,$(notdir $(BOOT_SRCS:.c=.o)))
BDEPS := $(BOBJS:.o=.d)

b_%.o: %.c
	@echo 'Building file: $<'
	$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"

$(TARGET).elf: $(BOBJS)
	@echo 'Linking bootloader: $@'
	$(LD) $(LDFLAGS) -o "$@" $(BOBJS)
	@echo 'Finished building: $@'

$(TARGET).hex: $(TARGET).elf
	@echo 'Creating flash image: $@'
	$(OBJCOPY) -O ihex "$<" "$@"

all: $(TARGET).elf $(TARGET).hex
	@echo 'Bootloader size:'
	$(SIZE) $(TARGET).elf

clean:
	-$(RM) $(BOBJS) $(BDEPS) "$(TARGET).elf" "$(TARGET).hex" "$(TARGET).map"

-include $(BDEPS)

.PHONY: all clean

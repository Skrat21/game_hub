set pagination off
set confirm off
set remotetimeout 300

# Bootloader symbols (linked at 0x08000000) + application (linked at
# 0x08008000). The two images do not overlap.
file "bootloader/bootloader.elf"
add-symbol-file "Debug/fixed project.elf"

target extended-remote localhost:3333
monitor reset halt
delete

# 'main' exists in both images, so this breakpoint covers the whole chain:
# bootloader main -> application main.
break main
commands
silent
printf "\n@@@ in main() at %p\n", $pc
if ($pc < 0x08008000)
  printf "    -> that is the BOOTLOADER main (Sectors 0-1)\n"
  printf "    app stack pointer    = %p\n", *(unsigned int*)0x08008000
  printf "    app reset vector     = %p\n", *(unsigned int*)0x08008004
  printf "    app metadata magic   = 0x%08x  (0x47504D31 = 'GPM1')\n", *(unsigned int*)0x0803FFF0
  printf "    app metadata version = %u\n", *(unsigned int*)0x0803FFF4
else
  printf "    -> that is the APPLICATION main (0x08008000+) - handover SUCCEEDED\n"
end
continue
end

break MENU_enChooseMode
commands
silent
printf "\n@@@ APPLICATION BOOTED: reached MENU_enChooseMode at %p (in the application region)\n", $pc
end

printf "\n=== reset and run the whole boot chain: bootloader -> application ===\n"
continue

printf "\n@@@ BOOT CHAIN COMPLETE - the bootloader validated and started the application\n"

detach
quit
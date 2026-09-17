set pagination off
set confirm off
set remotetimeout 180
target extended-remote localhost:3333
monitor halt
delete

printf "\n=== SAFETY CHECK: is the running image still intact after all the experiments? ===\n"
printf "0x08000000 (app MSP)  : "
x/1xw 0x08000000
printf "0x08000004 (reset vec): "
x/1xw 0x08000004
printf "0x08004000 (inside HESP8266_u8OpenTcpConnection): "
x/1xw 0x08004000
printf "0x08007e38 (OTA_vSetFlag, also inside Sector 1): "
x/1xw 0x08007e38
printf "0x08008288 (OTA_vCheckForUpdate, Sector 2)     : "
x/1xw 0x08008288 LI

printf "\n=== FMI driver: is the erase real? ===\n"
printf "MFMI_vSectorErase addr: "
print/x (void*)MFMI_vSectorErase
printf "MFMI_vProgramFlash addr: "
print/x (void*)MFMI_vProgramFlash
printf "FLASH_ACR   (0x40023C00) = "
x/1xw 0x40023C00
printf "FLASH_SR    (0x40023C0C) = "
x/1xw 0x40023C0C
printf "FLASH_CR    (0x40023C10) = "
x/1xw 0x40023C10

detach
quit
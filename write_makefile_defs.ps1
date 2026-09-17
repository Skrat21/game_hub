$recipe = "{0}@echo 'Building file: `$<'`n" -f $tab
$recipe += "{0}@echo 'Invoking: Cross ARM GNU C Compiler'`n" -f $tab
$recipe += "{0}arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I\"../include\" -I\"../system/include\" -I\"../system/include/cmsis\" -I\"../system/include/stm32f4-hal\" -std=gnu11 -O0 -MMD -MP -MF\"`$(@:%.o=%.d)\" -MT\"`$(@)\" -c -o \"`$@\" \"`$<'`n" -f $tab
$recipe += "{0}@echo 'Finished building: `$<'`n" -f $tab
$recipe += "{0}@echo ' '`n`n" -f $tab

function MakeRule($pattern) {
    return "$pattern`n$recipe"
}

$more = @(
    MakeRule("src/APP/TicTacToe/%.o: ../src/APP/TicTacToe/%.c"),
    MakeRule("src/APP/TicTacToe/Animation/%.o: ../src/APP/TicTacToe/Animation/%.c"),
    MakeRule("src/APP/Connect4/%.o: ../src/APP/Connect4/%.c"),
    MakeRule("src/APP/Connect4/Animation/%.o: ../src/APP/Connect4/Animation/%.c"),
    MakeRule("src/APP/Memory/%.o: ../src/APP/Memory/%.c"),
    MakeRule("src/APP/Memory/Animation/%.o: ../src/APP/Memory/Animation/%.c"),
    MakeRule("src/APP/Yatzy/%.o: ../src/APP/Yatzy/%.c"),
    MakeRule("src/APP/Yatzy/Animation/%.o: ../src/APP/Yatzy/Animation/%.c"),
    MakeRule("src/HAL/OTA_LED/%.o: ../src/HAL/OTA_LED/%.c"),
    "",
    "# ---------------------------------------------------------------------------",
    "# Windows \"make clean\" support",
    "# ---------------------------------------------------------------------------",
    "",
    "ifeq ($(OS),Windows_NT)",
    "RM := powershell -NoProfile -ExecutionPolicy Bypass -File ../tools/rm.ps1",
    "endif",
    ""
)

$lines = [System.IO.File]::ReadAllLines("$projectDir/makefile.defs")
$content = [string]::Join("`n", $lines) + "`n" + [string]::Join("`n", $more)
[System.IO.File]::WriteAllText("$projectDir/makefile.defs", $content)
Write-Host "Part 2 appended: $(($more.Count)) lines, total: $(($lines.Count + $more.Count))"

# Minimal "rm -rf" stand-in for the Eclipse-generated clean recipe.
#
# Debug/makefile runs:  -$(RM) $(OBJS) $(C_DEPS) ... "fixed project.elf"
# with RM defaulting to "rm -rf". A plain Windows install has no rm, and the
# leading "-" in the recipe makes make ignore the failure, so "make clean"
# reported "process_begin ... failed" and left every object file behind.
#
# makefile.defs therefore points RM at this script on Windows:
#   powershell -NoProfile -ExecutionPolicy Bypass -File ../tools/rm.ps1 <paths...>
#
# Unmatched or already-deleted paths are tolerated on purpose, mirroring "rm -rf".

foreach ($item in $args) {
    if ([string]::IsNullOrWhiteSpace($item)) {
        continue
    }
    Remove-Item -LiteralPath $item -Force -Recurse -ErrorAction SilentlyContinue
}

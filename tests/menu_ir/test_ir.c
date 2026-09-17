#include <assert.h>
#include <stdio.h>
#include "../../src/HAL/IR/IR_decode.h"

static HIR_Decoder_t decoder;
static HIR_Command_t command;
static u32 now;
static unsigned events;
static void edge(u32 interval)
{
    now += interval;
    events += HIR_u8DecoderEdge(&decoder, now, &command);
}
static void frame(u32 bits, u32 leader, u32 zero, u32 one)
{
    unsigned bit;
    edge(50000);
    edge(leader);
    for (bit = 0; bit < 32; bit++)
        edge((bits & ((u32)1 << bit)) ? one : zero);
}
int main(void)
{
    unsigned code;
    HIR_vDecoderReset(&decoder);
    /* Wire bytes 00 FF 07 F8: address 0, EQ/select command 07. */
    frame(0xF807FF00UL, 13500, 1125, 2250);
    assert(events == 1 && command.Address == 0 && command.Command == 0x07);
    puts("PASS: hardcoded NEC 00 FF 07 F8 -> address 00, command 07");
    for (code = 0; code < 256; code++) {
        u32 bits = 0xFF00UL | ((u32)code << 16) | ((u32)(code ^ 255) << 24);
        unsigned before = events;
        frame(bits, 12000, 900, 1800);
        assert(events == before + 1 && command.Command == code);
        frame(bits, 16000, 1400, 2600);
        assert(events == before + 2 && command.Command == code);
    }
    {
        unsigned before = events;
        frame(0xF807FE00UL, 13500, 1125, 2250); /* invalid address complement */
        frame(0xF907FF00UL, 13500, 1125, 2250); /* invalid command complement */
        frame(0xF807FF00UL, 11999, 1125, 2250);
        frame(0xF807FF00UL, 16001, 1125, 2250);
        frame(0xF807FF00UL, 13500, 899, 2250);
        frame(0xF807FF00UL, 13500, 1401, 2250);
        frame(0xF807FF00UL, 13500, 1125, 1799);
        frame(0xF807FF00UL, 13500, 1125, 2601);
        edge(50000); edge(11250); /* NEC short repeat */
        assert(events == before);
        edge(50000); edge(13500); edge(1125); edge(123); /* truncated/noise */
        frame(0xF807FF00UL, 13500, 1125, 2250);
        assert(events == before + 1);
        HIR_vDecoderReset(&decoder);
        now = 0xFFFF0000UL;
        frame(0xF807FF00UL, 13500, 1125, 2250);
        assert(events == before + 2 && command.Command == 7);
        frame(0xF807ED12UL, 13500, 1125, 2250);
        assert(events == before + 3 && command.Address == 0x12);
    }
    printf("PASS: all 256 commands at both timing bounds, invalid frames, repeats, recovery, wrap (%u events)\n", events);
    return 0;
}

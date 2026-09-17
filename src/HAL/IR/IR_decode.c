#include "IR_decode.h"

void HIR_vDecoderReset(HIR_Decoder_t *decoder)
{
    decoder->Timestamp = 0;
    decoder->Bits = 0;
    decoder->Count = 0;
    decoder->State = 0;
}

u8 HIR_u8DecoderEdge(HIR_Decoder_t *decoder, u32 now, HIR_Command_t *command)
{
    u32 interval = now - decoder->Timestamp;
    decoder->Timestamp = now;
    if (!decoder->State) {
        decoder->State = 1;
        return 0;
    }
    /* NEC: 9 ms mark + 4.5 ms space measured falling-to-falling.
     * Re-synchronize on any leader, including after noise/truncated frames.
     * Short NEC repeat leaders (~11.25 ms) intentionally produce no event,
     * matching the reference remote behavior. */
    if (interval >= 12000 && interval <= 16000) {
        decoder->Bits = 0;
        decoder->Count = 0;
        decoder->State = 2;
        return 0;
    }
    if (decoder->State == 2) {
        if (interval >= 1800 && interval <= 2600) {
            decoder->Bits |= (u32)1 << decoder->Count;
        } else if (interval < 900 || interval > 1400) {
            decoder->State = 1;
            return 0;
        }
        if (++decoder->Count == 32) {
            u8 address = (u8)decoder->Bits;
            u8 inverse_address = (u8)(decoder->Bits >> 8);
            u8 code = (u8)(decoder->Bits >> 16);
            u8 inverse_code = (u8)(decoder->Bits >> 24);
            decoder->State = 0;
            if ((u8)(address ^ inverse_address) == 0xFF &&
                (u8)(code ^ inverse_code) == 0xFF) {
                command->Address = address;
                command->Command = code;
                return 1;
            }
        }
    }
    return 0;
}

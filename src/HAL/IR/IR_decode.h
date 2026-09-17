#ifndef HAL_IR_IR_DECODE_H_
#define HAL_IR_IR_DECODE_H_
#include "IR_int.h"
/* Hardware-independent falling-edge decoder, also used by host tests. */
typedef struct {
    u32 Timestamp;
    u32 Bits;
    u8 Count;
    u8 State;
} HIR_Decoder_t;
void HIR_vDecoderReset(HIR_Decoder_t *decoder);
u8 HIR_u8DecoderEdge(HIR_Decoder_t *decoder, u32 now, HIR_Command_t *command);
#endif

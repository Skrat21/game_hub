/*
 * HEXPARSER_int.h
 *
 *  Created on: Sep 10, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_HEXPARSER_HEXPARSER_INT_H_
#define APP_HEXPARSER_HEXPARSER_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Existing API: parse a complete HEX record string */
void AHexParser_vParseRecord(const u8* A_s8Record);
void AHexParser_vParseData(const u8* A_s8Record);

/* Streaming API: feed one byte at a time, get decoded data back */
void AHexParser_vInitStream(void);

/*
 * Feed one byte from the HEX stream.
 * Returns 1 when a complete record has been parsed and decoded data
 * is available in outData (up to *outLen bytes). Returns 0 if more
 * bytes are needed to complete the current record.
 * outLen is set to the number of decoded bytes (max 255).
 * outData must point to a buffer of at least 256 bytes.
 */
u8 AHexParser_vProcessByte(u8 byte, u8 *outData, u8 *outLen);

/* Parse the address from the current/last record (for flash offset) */
u32 AHexParser_u32GetCurrentAddress(void);

/* Check if the last parsed record was an EOF record */
u8 AHexParser_u8IsEofRecord(void);

#endif /* APP_HEXPARSER_HEXPARSER_INT_H_ */

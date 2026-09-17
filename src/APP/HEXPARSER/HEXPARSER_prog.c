#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/FMI/FMI_int.h"
#include "HEXPARSER_int.h"

#define G_HIGH_ADDRESS 0x08000000

/* Streaming parser state */
#define HEX_MAX_RECORD_LEN 256
static u8 G_au8RecordBuffer[HEX_MAX_RECORD_LEN];
static u8 G_u8RecordIndex = 0;
static u8 G_u8RecordComplete = 0;
static u32 G_u32LastAddress = 0;
static u8 G_u8LastEof = 0;

static u8 AHexParser_ASCII_to_Hex(u8 A_u8ASCII) {
	u8 L_u8Value = 0;
	if (A_u8ASCII >= '0' && A_u8ASCII <= '9') {
		L_u8Value = A_u8ASCII - '0';
	} else if (A_u8ASCII >= 'A' && A_u8ASCII <= 'F') {
		L_u8Value = A_u8ASCII - 55;
	} else if (A_u8ASCII >= 'a' && A_u8ASCII <= 'f') {
		L_u8Value = A_u8ASCII - 87;
	}
	return L_u8Value;
}

void AHexParser_vParseRecord(const u8* A_s8Record) {
	u8 L_u8Var = AHexParser_ASCII_to_Hex(A_s8Record[8]);

	switch (L_u8Var) {
	case 0:
		AHexParser_vParseData(A_s8Record);
		break;
	case 1:
		// EOF
		break;
	case 2:
		// Extended address (not handled in original)
		break;
	}
}

void AHexParser_vParseData(const u8* A_s8Record) {
	u8 CC_high = AHexParser_ASCII_to_Hex(A_s8Record[1]);
	u8 CC_low = AHexParser_ASCII_to_Hex(A_s8Record[2]);

	u8 CC = (CC_high << 4) | (CC_low);

	// LOW ADDRESS
	u8 Address0 = AHexParser_ASCII_to_Hex(A_s8Record[3]);
	u8 Address1 = AHexParser_ASCII_to_Hex(A_s8Record[4]);
	u8 Address2 = AHexParser_ASCII_to_Hex(A_s8Record[5]);
	u8 Address3 = AHexParser_ASCII_to_Hex(A_s8Record[6]);

	u16 LowAddress = (Address0 << 12) | (Address1 << 8) | (Address2 << 4) | (Address3);

	u32 Address = G_HIGH_ADDRESS | LowAddress;

	u16 Data[16] = { 0 };

	for (u8 i = 0; i < (CC / 2); i++) {
		u8 digit0 = AHexParser_ASCII_to_Hex(A_s8Record[9  + (4 * i)]);
		u8 digit1 = AHexParser_ASCII_to_Hex(A_s8Record[10 + (4 * i)]);
		u8 digit2 = AHexParser_ASCII_to_Hex(A_s8Record[11 + (4 * i)]);
		u8 digit3 = AHexParser_ASCII_to_Hex(A_s8Record[12 + (4 * i)]);
		Data[i] = (digit0 <<4)  | (digit1 << 0)  | (digit2 << 12) | (digit3 << 8);
	}

	MFMI_vProgramFlash(Address,Data,(CC/2));
}

/* ============================================================
 * Streaming Hex Parser Implementation
 * ============================================================ */

void AHexParser_vInitStream(void)
{
	G_u8RecordIndex = 0;
	G_u8RecordComplete = 0;
	G_u32LastAddress = 0;
	G_u8LastEof = 0;
}

u32 AHexParser_u32GetCurrentAddress(void)
{
	return G_u32LastAddress;
}

u8 AHexParser_u8IsEofRecord(void)
{
	return G_u8LastEof;
}

/*
 * Process a single byte from the HEX stream.
 * Accumulates bytes into G_au8RecordBuffer until a complete record
 * (terminated by '\n') is received. When complete, parses the record
 * and outputs decoded binary data.
 *
 * Returns 1 if a complete record was parsed (outData filled), 0 otherwise.
 */
u8 AHexParser_vProcessByte(u8 byte, u8 *outData, u8 *outLen)
{
	/* Start of a new record */
	if (byte == ':')
	{
		G_u8RecordIndex = 0;
		G_u8RecordComplete = 0;
		G_au8RecordBuffer[G_u8RecordIndex++] = ':';
		return 0;
	}

	/* End of record */
	if (byte == '\n' || byte == '\r')
	{
		if (G_u8RecordIndex > 0 && G_au8RecordBuffer[0] == ':')
		{
			/* Null-terminate the record */
			G_au8RecordBuffer[G_u8RecordIndex] = '\0';
			G_u8RecordComplete = 1;
		}
		return 0;
	}

	/* Accumulate byte into record buffer */
	if (G_u8RecordIndex < HEX_MAX_RECORD_LEN - 1)
	{
		G_au8RecordBuffer[G_u8RecordIndex++] = byte;
	}

	if (!G_u8RecordComplete)
	{
		return 0;
	}

	/* We have a complete record — parse it */
	G_u8RecordComplete = 0;

	/* Parse byte count (positions 1-2) */
	u8 CC_high = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[1]);
	u8 CC_low = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[2]);
	u8 CC = (CC_high << 4) | CC_low;

	/* Parse record type (positions 7-8) */
	u8 rec_type_high = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[7]);
	u8 rec_type_low = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[8]);
	u8 rec_type = (rec_type_high << 4) | rec_type_low;

	/* Handle record type */
	if (rec_type == 1)
	{
		/* EOF record */
		G_u8LastEof = 1;
		*outLen = 0;
		return 1;
	}

	if (rec_type == 0 && CC > 0)
	{
		/* Data record — decode binary data */
		u8 Address0 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[3]);
		u8 Address1 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[4]);
		u8 Address2 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[5]);
		u8 Address3 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[6]);

		u16 LowAddress = (Address0 << 12) | (Address1 << 8) | (Address2 << 4) | Address3;
		G_u32LastAddress = G_HIGH_ADDRESS | LowAddress;

		/* Decode data bytes (skip the checksum at the end) */
		u8 data_byte_count = CC;
		u8 out_idx = 0;

		for (u8 i = 0; i < data_byte_count && out_idx < 255; i++)
		{
			u8 pos = 9 + (i * 2);
			u8 digit0 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[pos]);
			u8 digit1 = AHexParser_ASCII_to_Hex(G_au8RecordBuffer[pos + 1]);
			outData[out_idx++] = (digit0 << 4) | digit1;
		}

		*outLen = out_idx;
		return 1;
	}

	/* Non-data record (e.g., extended address) — no output */
	*outLen = 0;
	return 1;
}



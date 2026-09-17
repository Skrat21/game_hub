#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/USART/USART_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "ESP8266_int.h"

#define HESP8266_COMMAND_TIMEOUT_MS 5000
#define HESP8266_WIFI_TIMEOUT_MS    15000
#define HESP8266_HTTP_TIMEOUT_MS    10000

static u8 G_u8TcpConnected = 0;
static u8 G_u8WifiConnected = 0;
volatile HESP8266_DebugStage_t G_xHESP8266DebugStage = HESP8266_DEBUG_RESET;
volatile u8 G_u8HESP8266LastStatus = HESP8266_STATUS_OK;
volatile char G_cHESP8266LastResponse[64];

static void SaveResponse(const volatile u8 *response)
{
	u8 index;

	for (index = 0; index < sizeof(G_cHESP8266LastResponse) - 1
			&& response[index] != '\0'; index++) {
		G_cHESP8266LastResponse[index] = (char) response[index];
	}
	G_cHESP8266LastResponse[index] = '\0';
}

static u8 StringContains(const char *text, const char *token)
{
	if (text == NULL || token == NULL) {
		return 0;
	}

	while (*text != '\0') {
		const char *text_cursor = text;
		const char *token_cursor = token;

		while (*text_cursor != '\0' && *token_cursor != '\0'
				&& *text_cursor == *token_cursor) {
			text_cursor++;
			token_cursor++;
		}

		if (*token_cursor == '\0') {
			return 1;
		}
		text++;
	}

	return 0;
}

static u16 StringLength(const char *text)
{
	u16 length = 0;

	if (text != NULL) {
		while (text[length] != '\0') {
			length++;
		}
	}

	return length;
}

static void UIntToString(u16 value, char *output)
{
	char reversed[6];
	u8 index = 0;
	u8 output_index;

	if (value == 0) {
		output[0] = '0';
		output[1] = '\0';
		return;
	}

	while (value > 0 && index < sizeof(reversed)) {
		reversed[index++] = (char) ((value % 10) + '0');
		value /= 10;
	}

	for (output_index = 0; output_index < index; output_index++) {
		output[output_index] = reversed[index - output_index - 1];
	}
	output[index] = '\0';
}

static HESP8266_Status_t WaitForToken(const char *token, u32 timeout_ms)
{
	u8 data;
	char line[128];
	u16 line_index = 0;
	u32 elapsed = 0;

	while (elapsed < timeout_ms) {
		while (MUSART_u8ReadReceivedByte(&data)) {
			if (data == '>') {
				if (StringContains(token, ">")) {
					return HESP8266_STATUS_OK;
				}
				continue;
			}

			if (data == '\r' || data == '\n') {
				line[line_index] = '\0';
				SaveResponse((const volatile u8 *) line);
				if (StringContains(line, "ERROR")
						|| StringContains(line, "FAIL")) {
					return HESP8266_STATUS_ERROR;
				}
				if (StringContains(line, token)) {
					return HESP8266_STATUS_OK;
				}
				line_index = 0;
			} else if (line_index < sizeof(line) - 1) {
				line[line_index++] = (char) data;
			}
		}

		MSYSTICK_vSetDelay_ms(10);
		elapsed += 10;
	}

	return HESP8266_STATUS_TIMEOUT;
}

static HESP8266_Status_t WaitForHttpBody(char *body, u16 body_size)
{
	u8 data;
	char line[256];
	u16 line_index = 0;
	u32 elapsed = 0;

	while (elapsed < HESP8266_HTTP_TIMEOUT_MS) {
		while (MUSART_u8ReadReceivedByte(&data)) {
			if (data == '\r' || data == '\n') {
				line[line_index] = '\0';
				SaveResponse((const volatile u8 *) line);

				if (StringContains(line, "state=")
						|| StringContains(line, "status=")) {
					u16 index = 0;
					while (line[index] != '\0' && index < body_size - 1) {
						body[index] = line[index];
						index++;
					}
					body[index] = '\0';
					return HESP8266_STATUS_OK;
				}

				if (StringContains(line, "ERROR")) {
					return HESP8266_STATUS_ERROR;
				}
				line_index = 0;
			} else if (line_index < sizeof(line) - 1) {
				line[line_index++] = (char) data;
			}
		}

		MSYSTICK_vSetDelay_ms(10);
		elapsed += 10;
	}

	return HESP8266_STATUS_TIMEOUT;
}

static HESP8266_Status_t SendCommand(const char *command,
		const char *expected, u32 timeout_ms)
{
	MUSART_vClearReceiveBuffer();
	MUSART_vSendString((char *) command);
	G_u8HESP8266LastStatus = WaitForToken(expected, timeout_ms);
	return G_u8HESP8266LastStatus;
}

static u16 AppendPart(char *destination, u16 index, u16 capacity,
		const char *part)
{
	if (part == NULL) {
		return index;
	}

	while (*part != '\0' && index < capacity - 1) {
		destination[index++] = *part++;
	}

	return index;
}

void HESP8266_vInit(void)
{
	G_xHESP8266DebugStage = HESP8266_DEBUG_RESET;
	MUSART_vInit();
	G_xHESP8266DebugStage = HESP8266_DEBUG_USART_READY;
	MUSART_vClearReceiveBuffer();
	G_u8TcpConnected = 0;
	G_u8WifiConnected = 0;
	SendCommand("ATE0\r\n", "OK", HESP8266_COMMAND_TIMEOUT_MS);
	G_xHESP8266DebugStage = HESP8266_DEBUG_ATE0_SENT;
	SendCommand("AT+CWMODE=1\r\n", "OK", HESP8266_COMMAND_TIMEOUT_MS);
	G_xHESP8266DebugStage = HESP8266_DEBUG_MODE_SENT;
}

HESP8266_Status_t HESP8266_u8ConnectAccessPoint(const char *ssid,
		const char *password)
{
	char command[160];
	u16 index = 0;

	index = AppendPart(command, index, sizeof(command), "AT+CWJAP=\"");
	index = AppendPart(command, index, sizeof(command), ssid);
	index = AppendPart(command, index, sizeof(command), "\",\"");
	index = AppendPart(command, index, sizeof(command), password);
	index = AppendPart(command, index, sizeof(command), "\"\r\n");
	command[index] = '\0';

	G_xHESP8266DebugStage = HESP8266_DEBUG_WIFI_SENT;
	{
		HESP8266_Status_t status = SendCommand(command, "OK", HESP8266_WIFI_TIMEOUT_MS);
		G_u8WifiConnected = (status == HESP8266_STATUS_OK);
		if (status == HESP8266_STATUS_OK) {
			G_xHESP8266DebugStage = HESP8266_DEBUG_WIFI_READY;
		}
		return status;
	}
}

HESP8266_Status_t HESP8266_u8OpenTcpConnection(const char *ip,
		const char *port)
{
	char command[96];
	u16 index = 0;
	HESP8266_Status_t status;

	index = AppendPart(command, index, sizeof(command),
			"AT+CIPSTART=\"TCP\",\"");
	index = AppendPart(command, index, sizeof(command), ip);
	index = AppendPart(command, index, sizeof(command), "\",");
	index = AppendPart(command, index, sizeof(command), port);
	index = AppendPart(command, index, sizeof(command), "\r\n");
	command[index] = '\0';

	status = SendCommand(command, "OK", HESP8266_COMMAND_TIMEOUT_MS);
	G_u8TcpConnected = (status == HESP8266_STATUS_OK);
	if (G_u8TcpConnected) {
		G_xHESP8266DebugStage = HESP8266_DEBUG_TCP_READY;
	}
	return status;
}

HESP8266_Status_t HESP8266_u8EnsureConnection(const char *ssid,
		const char *password, const char *ip, const char *port, u8 retries)
{
	u8 attempt;
	HESP8266_Status_t status = HESP8266_STATUS_ERROR;

	for (attempt = 0; attempt < retries; attempt++) {
		if (!G_u8WifiConnected) {
			status = HESP8266_u8ConnectAccessPoint(ssid, password);
		}
		if (G_u8WifiConnected) {
			status = HESP8266_u8OpenTcpConnection(ip, port);
			if (status == HESP8266_STATUS_OK) {
				return HESP8266_STATUS_OK;
			}
		}

		HESP8266_vAbort();
		MSYSTICK_vSetDelay_ms(1000);
	}

	return status;
}

HESP8266_Status_t HESP8266_u8HttpGet(const char *path, char *body,
		u16 body_size)
{
	char request[160];
	char length_string[6];
	u16 request_length;
	u16 index = 0;

	if (!G_u8TcpConnected || path == NULL || body == NULL || body_size < 2) {
		return HESP8266_STATUS_NOT_CONNECTED;
	}

	index = AppendPart(request, index, sizeof(request), "GET ");
	index = AppendPart(request, index, sizeof(request), path);
	index = AppendPart(request, index, sizeof(request),
			" HTTP/1.1\r\nHost: jozzzef.atwebpages.com\r\nConnection: close\r\n\r\n");
	request[index] = '\0';

	request_length = StringLength(request);
	UIntToString(request_length, length_string);

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString("AT+CIPSEND=");
	MUSART_vSendString(length_string);
	MUSART_vSendString("\r\n");
	if (WaitForToken(">", HESP8266_COMMAND_TIMEOUT_MS)
			!= HESP8266_STATUS_OK) {
		HESP8266_vAbort();
		return HESP8266_STATUS_ERROR;
	}

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString(request);
	body[0] = '\0';

	if (WaitForHttpBody(body, body_size) == HESP8266_STATUS_OK) {
		return HESP8266_STATUS_OK;
	}

		HESP8266_vAbort();
	return HESP8266_STATUS_TIMEOUT;
}

/*
 * Text HTTP GET support.
 *
 * HESP8266_u8HttpGet() only returns a line that contains "state=" or
 * "status=" (the game status protocol), and only once a CR/LF terminates
 * it. The OTA trigger file /status/ota.txt is a plain "key=value;..." line
 * with neither token and no trailing line terminator, so it can never come
 * back through that path. This variant returns whatever text body the
 * server sends, with the HTTP headers skipped.
 */
#define HESP8266_BODY_IDLE_MS 200

/* The ESP8266 appends "CLOSED" to the relayed body of a Connection: close
 * response; drop it (and any trailing CR/LF) so callers see just the body. */
static void StripTrailingClosed(char *body, u16 *length)
{
	static const char closed[] = "CLOSED";
	u16 body_length = *length;
	u16 token_length = (u16) (sizeof(closed) - 1);
	u16 index;

	while (body_length > 0
			&& (body[body_length - 1] == '\r'
					|| body[body_length - 1] == '\n')) {
		body_length--;
	}

	if (body_length >= token_length) {
		u16 start = (u16) (body_length - token_length);
		u8 matches = 1;

		for (index = 0; index < token_length; index++) {
			if (body[start + index] != closed[index]) {
				matches = 0;
			}
		}
		if (matches) {
			body_length = start;
		}
	}

	body[body_length] = '\0';
	*length = body_length;
}

static HESP8266_Status_t WaitForHttpBodyText(char *body, u16 body_size)
{
	u8 data;
	u32 elapsed = 0;
	u8 header_end = 0;
	u8 last_byte = 0;
	u8 prev_byte = 0;
	u16 body_length = 0;

	body[0] = '\0';

	while (elapsed < HESP8266_HTTP_TIMEOUT_MS) {
		u8 received = 0;

		while (MUSART_u8ReadReceivedByte(&data)) {
			received = 1;

			if (!header_end) {
				/* The '\n' that completes "\r\n\r\n" ends the headers. */
				if (last_byte == '\r' && prev_byte == '\n'
						&& data == '\n') {
					header_end = 1;
					last_byte = 0;
					prev_byte = 0;
					continue;
				}
				prev_byte = last_byte;
				last_byte = data;
				continue;
			}

			if (body_length < body_size - 1) {
				body[body_length++] = (char) data;
			}
		}

		if (!received) {
			if (header_end && body_length > 0) {
				/* Body went quiet - treat it as complete. */
				MSYSTICK_vSetDelay_ms(HESP8266_BODY_IDLE_MS);
				elapsed += HESP8266_BODY_IDLE_MS;
				break;
			}
			MSYSTICK_vSetDelay_ms(10);
			elapsed += 10;
		}
	}

	body[body_length] = '\0';
	StripTrailingClosed(body, &body_length);

	return (body_length > 0) ? HESP8266_STATUS_OK
			: HESP8266_STATUS_TIMEOUT;
}

HESP8266_Status_t HESP8266_u8HttpGetText(const char *path, char *body,
		u16 body_size)
{
	char request[160];
	char length_string[6];
	u16 request_length;
	u16 index = 0;

	if (!G_u8TcpConnected || path == NULL || body == NULL || body_size < 2) {
		return HESP8266_STATUS_NOT_CONNECTED;
	}

	index = AppendPart(request, index, sizeof(request), "GET ");
	index = AppendPart(request, index, sizeof(request), path);
	index = AppendPart(request, index, sizeof(request),
			" HTTP/1.1\r\nHost: jozzzef.atwebpages.com\r\nConnection: close\r\n\r\n");
	request[index] = '\0';

	request_length = StringLength(request);
	UIntToString(request_length, length_string);

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString("AT+CIPSEND=");
	MUSART_vSendString(length_string);
	MUSART_vSendString("\r\n");
	if (WaitForToken(">", HESP8266_COMMAND_TIMEOUT_MS)
			!= HESP8266_STATUS_OK) {
		HESP8266_vAbort();
		return HESP8266_STATUS_ERROR;
	}

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString(request);

	if (WaitForHttpBodyText(body, body_size) == HESP8266_STATUS_OK) {
		return HESP8266_STATUS_OK;
	}

	HESP8266_vAbort();
	return HESP8266_STATUS_TIMEOUT;
}

/*
 * Streaming HTTP GET: reads the full HTTP response body as raw bytes,
 * skipping headers, and passes data chunks to the callback.
 * Unlike HESP8266_u8HttpGet(), this is binary-safe and does not assume
 * newline-delimited text.
 */
static HESP8266_Status_t WaitForHttpBodyStream(
		void (*callback)(const u8 *data, u16 len))
{
	u8 data;
	u32 elapsed = 0;
	u8 header_end = 0;
	char last_byte = 0;
	char prev_byte = 0;
	u8 chunk[64];
	u16 chunk_len = 0;

	while (elapsed < HESP8266_HTTP_TIMEOUT_MS) {
		/* Read all available bytes from ring buffer */
		while (MUSART_u8ReadReceivedByte(&data)) {
			if (!header_end) {
				/* Look for \r\n\r\n (end of HTTP headers) */
				if (last_byte == '\r' && prev_byte == '\n') {
					if (data == '\n') {
						/* Found \r\n\r\n — headers ended */
						header_end = 1;
						last_byte = 0;
						prev_byte = 0;
						continue;
					}
				}
				prev_byte = last_byte;
				last_byte = data;
				continue;
			}

			/* We are in the body — accumulate for callback */
			if (chunk_len < sizeof(chunk)) {
				chunk[chunk_len++] = data;
			} else {
				/* Flush chunk */
				callback(chunk, chunk_len);
				chunk_len = 0;
				chunk[chunk_len++] = data;
			}
		}

		/* Check for CLOSED connection (no data for a while means done) */
		if (header_end && MUSART_u8ReadReceivedByte(NULL) == 0) {
			MSYSTICK_vSetDelay_ms(100);
			elapsed += 100;
			/* After headers, if no data received for a while, we're done */
			if (elapsed > HESP8266_HTTP_TIMEOUT_MS / 2 && chunk_len > 0) {
				callback(chunk, chunk_len);
				chunk_len = 0;
			}
			if (elapsed >= HESP8266_HTTP_TIMEOUT_MS) {
				break;
			}
		} else {
			MSYSTICK_vSetDelay_ms(10);
			elapsed += 10;
		}

		/* Check for error in response */
		if (StringContains((const char*)chunk, "ERROR") && chunk_len > 0) {
			callback(chunk, chunk_len);
			chunk_len = 0;
		}
	}

	/* Flush any remaining data */
	if (chunk_len > 0 && header_end) {
		callback(chunk, chunk_len);
	}

	if (header_end) {
		return HESP8266_STATUS_OK;
	}
	return HESP8266_STATUS_TIMEOUT;
}

HESP8266_Status_t HESP8266_u8HttpGetStream(const char *path,
		void (*callback)(const u8 *data, u16 len))
{
	char request[160];
	char length_string[6];
	u16 request_length;
	u16 index = 0;

	if (!G_u8TcpConnected || path == NULL || callback == NULL) {
		return HESP8266_STATUS_NOT_CONNECTED;
	}

	index = AppendPart(request, index, sizeof(request), "GET ");
	index = AppendPart(request, index, sizeof(request), path);
	index = AppendPart(request, index, sizeof(request),
			" HTTP/1.1\r\nHost: jozzzef.atwebpages.com\r\nConnection: close\r\n\r\n");
	request[index] = '\0';

	request_length = StringLength(request);
	UIntToString(request_length, length_string);

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString("AT+CIPSEND=");
	MUSART_vSendString(length_string);
	MUSART_vSendString("\r\n");
	if (WaitForToken(">", HESP8266_COMMAND_TIMEOUT_MS)
			!= HESP8266_STATUS_OK) {
		HESP8266_vAbort();
		return HESP8266_STATUS_ERROR;
	}

	MUSART_vClearReceiveBuffer();
	MUSART_vSendString(request);

	if (WaitForHttpBodyStream(callback) == HESP8266_STATUS_OK) {
		return HESP8266_STATUS_OK;
	}

	HESP8266_vAbort();
	return HESP8266_STATUS_TIMEOUT;
}

void HESP8266_vCloseConnection(void)
{
	if (G_u8TcpConnected) {
		SendCommand("AT+CIPCLOSE\r\n", "OK", HESP8266_COMMAND_TIMEOUT_MS);
	}
	G_u8TcpConnected = 0;
	MUSART_vClearReceiveBuffer();
}

void HESP8266_vAbort(void)
{
	MUSART_vClearReceiveBuffer();
	MUSART_vSendString("+++\r\n");
	MSYSTICK_vSetDelay_ms(100);
	HESP8266_vCloseConnection();
}

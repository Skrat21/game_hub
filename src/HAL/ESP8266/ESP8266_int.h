/*
 * ESP8266_int.h
 *
 *  Created on: Sep 8, 2026
 *      Author: AHMED SHERIF
 */

#ifndef HAL_ESP8266_ESP8266_INT_H_
#define HAL_ESP8266_ESP8266_INT_H_

#include "../../LIB/STD_TYPES.h"

typedef enum {
	HESP8266_DEBUG_RESET = 0,
	HESP8266_DEBUG_USART_READY,
	HESP8266_DEBUG_ATE0_SENT,
	HESP8266_DEBUG_MODE_SENT,
	HESP8266_DEBUG_WIFI_SENT,
	HESP8266_DEBUG_WIFI_READY,
	HESP8266_DEBUG_TCP_READY,
	HESP8266_DEBUG_MAIN_STATUS_SENT,
	HESP8266_DEBUG_MAIN_STATUS_RECEIVED,
	HESP8266_DEBUG_RPS_START
} HESP8266_DebugStage_t;

extern volatile HESP8266_DebugStage_t G_xHESP8266DebugStage;
extern volatile u8 G_u8HESP8266LastStatus;
extern volatile char G_cHESP8266LastResponse[64];

typedef enum {
	HESP8266_STATUS_OK = 0,
	HESP8266_STATUS_TIMEOUT,
	HESP8266_STATUS_ERROR,
	HESP8266_STATUS_NOT_CONNECTED
} HESP8266_Status_t;

void HESP8266_vInit(void);
HESP8266_Status_t HESP8266_u8ConnectAccessPoint(const char *ssid,
		const char *password);
HESP8266_Status_t HESP8266_u8OpenTcpConnection(const char *ip,
		const char *port);
HESP8266_Status_t HESP8266_u8EnsureConnection(const char *ssid,
		const char *password, const char *ip, const char *port, u8 retries);
HESP8266_Status_t HESP8266_u8HttpGet(const char *path, char *body,
		u16 body_size);
HESP8266_Status_t HESP8266_u8HttpGetText(const char *path, char *body,
		u16 body_size);
HESP8266_Status_t HESP8266_u8HttpGetStream(const char *path,
		void (*callback)(const u8 *data, u16 len));
void HESP8266_vCloseConnection(void);
void HESP8266_vAbort(void);


#endif /* HAL_ESP8266_ESP8266_INT_H_ */

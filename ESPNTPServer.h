/*
 * ESPNTPServer.h
 *
 * Copyright 2017 Christopher B. Liebman
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 *  Created on: Oct 29, 2017
 *      Author: liebman
 */

#define USE_ASYNC_UDP
#define USE_OLED_DISPLAY
//#define USE_NO_WIFI

#ifndef _ESPNTPServer_H_
#define _ESPNTPServer_H_
#include "Arduino.h"
#include "Ticker.h"
#if !defined(USE_NO_WIFI)
#include "WiFiManager.h"
#endif
#include "SoftwareSerial.h"
#include "MicroNMEA.h"
#include <time.h>

#if defined(USE_OLED_DISPLAY)
#include "SSD1306Wire.h"
#endif

#if defined(USE_ASYNC_UDP)
#include "ESPAsyncUDP.h"
#else
#include <WiFiUdp.h>
#endif

// pin definitions
#define LED_PIN                BUILTIN_LED   // LED on pin, active low
#define SYNC_PIN               D5   // (GPIO14) pin tied to 1hz square wave from GPS
#define GPS_RX_PIN             D6   // (GPIO12)
#define GPS_TX_PIN             D7   // (GPIO13)
#define GPS_EN_PIN             D4   // (GPIO2)

#define NTP_PORT               123

#define SYNC_EDGE_RISING       1
#define SYNC_EDGE_FALLING      0

#define WARMUP_SECONDS         2
#define PRECISION_COUNT        10000
#define MICROS_PER_SEC         1000000
#define SERIAL_BUFFER_SIZE     128
#define NMEA_BUFFER_SIZE       128
#define VALIDITY_CHECK_MS      1100
//#define MICROS_HISTORY_SIZE    1000

#define us2s(x) (((double)x)/(double)MICROS_PER_SEC) // microseconds to seconds

typedef struct ntp_time
{
    uint32_t seconds;
    uint32_t fraction;
} NTPTime;

typedef struct ntp_packet
{
    uint8_t  flags;
    uint8_t  stratum;
    uint8_t  poll;
    int8_t   precision;
    uint32_t delay;
    uint32_t dispersion;
    uint8_t  ref_id[4];
    NTPTime  ref_time;
    NTPTime  orig_time;
    NTPTime  recv_time;
    NTPTime  xmit_time;
} NTPPacket;

#define LI_NONE         0
#define LI_SIXTY_ONE    1
#define LI_FIFTY_NINE   2
#define LI_NOSYNC       3

#define MODE_RESERVED   0
#define MODE_ACTIVE     1
#define MODE_PASSIVE    2
#define MODE_CLIENT     3
#define MODE_SERVER     4
#define MODE_BROADCAST  5
#define MODE_CONTROL    6
#define MODE_PRIVATE    7

#define NTP_VERSION     4

#define REF_ID          "PPS "  // "GPS " when we have one!

#define setLI(value)    ((value&0x03)<<6)
#define setVERS(value)  ((value&0x07)<<3)
#define setMODE(value)  ((value&0x07))

#define getLI(value)    ((value>>6)&0x03)
#define getVERS(value)  ((value>>3)&0x07)
#define getMODE(value)  (value&0x07)

#define SEVENTY_YEARS   2208988800L
#define toEPOCH(t)      ((uint32_t)t-SEVENTY_YEARS)
#define toNTP(t)        ((uint32_t)t+SEVENTY_YEARS)

//  simple versions - we don't worry about side effects
#define MAX(a, b)   ((a) < (b) ? (b) : (a))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))

#endif /* _ESPNTPServer_H_ */

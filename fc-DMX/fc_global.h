/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Flux Capacitor - DMX-controlled
 * (C) 2024 Thomas Winischhofer (A10001986)
 * All rights reserved.
 * -------------------------------------------------------------------
 */

#ifndef _FC_GLOBAL_H
#define _FC_GLOBAL_H

/*************************************************************************
 ***                          Version Strings                          ***
 *************************************************************************/

#define FC_VERSION "V1.01"
#define FC_VERSION_EXTRA "MAR072024"

#define FC_DBG              // debug output on Serial

// If this is uncommented, the firmware uses channel DMX_VERIFY_CHANNEL
// for packet verification. The value of this channel must, at all times,
// be DMX_VERIFY_VALUE for a packet to be accepted.
#define DMX_USE_VERIFY

/*************************************************************************
 ***                             GPIO pins                             ***
 *************************************************************************/

// FC LEDs
#define SHIFT_CLK_PIN     21
#define REG_CLK_PIN       18
#define MRESET_PIN        19
#define SERDATA_PIN       22

// Box LEDs
#define BLED_PWM_PIN      2

// Center LED
#define LED_PWM_PIN       17

// GPIO14 (for alternative box lights)
#define GPIO_14           14

// IR Remote input
#define IRREMOTE_PIN      27

// IR feedback LED
#ifndef BOARD_1_2
#define IR_FB_PIN         12
#else
#define IR_FB_PIN         14
#endif

// Time Travel button (or TCD tt trigger input)
#define TT_IN_PIN         13

// I2S audio pins
#define I2S_BCLK_PIN      26
#define I2S_LRCLK_PIN     25
#define I2S_DIN_PIN       33

// SD Card pins
#define SD_CS_PIN          5
#define SPI_MOSI_PIN      23
#define SPI_MISO_PIN      16
#define SPI_SCK_PIN       15

// analog input pin, used for Speed
#define SPEED_PIN         32   

// analog input, for volume (control board v1.3+)
#define VOLUME_PIN        35    

// DMX
#define DMX_TRANSMIT 14
#define DMX_RECEIVE  13
#define DMX_ENABLE   32

#endif

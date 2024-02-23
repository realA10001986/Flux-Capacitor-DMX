/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Flux Capacitor - DMX-controlled
 * (C) 2024 Thomas Winischhofer (A10001986)
 * All rights reserved.
 * -------------------------------------------------------------------
 */

#include "fc_global.h"

#include <Arduino.h>
#include <esp_dmx.h>

#include "fc_dmx.h"
#include "fcdisplay.h"

// CenterLED PWM properties
#define CLED_FREQ     5000
#define CLED_CHANNEL  0
#define CLED_RES      8

// BoxLED PWM properties
#define BLED_FREQ     5000
#define BLED_CHANNEL  1
#define BLED_RES      8

#define POT_GRAN       45
static const uint16_t potSpeeds[POT_GRAN] = {
      2,   3,   3,   4,   5,   6,   7,   8,   9,  10,
     11,  12,  13,  14,  15,  16,  17,  18,  19,  20, 
     25,  30,  35,  40,  45,  50,  55,  60,  65,  70, 
     80,  90, 100, 110, 120, 140, 160, 180, 200, 250,
    300, 350, 400, 450, 500
};

// The center LED object
static PWMLED centerLED(LED_PWM_PIN);

// The Box LED object
static PWMLED boxLED(BLED_PWM_PIN);

// The FC LEDs object
FCLEDs fcLEDs(1, SHIFT_CLK_PIN, REG_CLK_PIN, SERDATA_PIN, MRESET_PIN);

int transmitPin = DMX_TRANSMIT;
int receivePin = DMX_RECEIVE;
int enablePin = DMX_ENABLE;

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = 1;

dmx_packet_t packet;

uint8_t data[DMX_PACKET_SIZE];

// DMX footprints for the displays
#define FC_BASE 36

unsigned long powerupMillis;

static bool          dmxIsConnected = false;
static unsigned long lastDMXpacket;

static void setDisplay(int base);


/*********************************************************************************
 * 
 * boot
 *
 *********************************************************************************/

void dmx_boot() 
{
    // Boot center LED here (is for some reason on after reset)
    #ifdef FC_DBG
    Serial.println(F("Booting Center LED"));
    #endif
    centerLED.begin(CLED_CHANNEL, CLED_FREQ, CLED_RES);

    // Boot FC leds here to have a way to show the user whats going on
    #ifdef FC_DBG
    Serial.println(F("Booting FC LEDs"));
    #endif
    fcLEDs.begin();

    // Boot remaining display LEDs (but keep them dark)
    #ifdef FC_DBG
    Serial.println(F("Booting Box LED"));
    #endif
    boxLED.begin(BLED_CHANNEL, BLED_FREQ, BLED_RES, 255);

    // Make sure LEDs are off
    centerLED.setDC(0);
    boxLED.setDC(0);

    // Init and turn off IR feedback LED
    pinMode(IR_FB_PIN, OUTPUT);
    digitalWrite(IR_FB_PIN, LOW);
}    



/*********************************************************************************
 * 
 * setup
 *
 *********************************************************************************/


void dmx_setup() 
{
    dmx_config_t config = {
      .interrupt_flags = DMX_INTR_FLAGS_DEFAULT,
      .root_device_parameter_count = 32,
      .sub_device_parameter_count = 0,
      .model_id = 0,
      .product_category = RDM_PRODUCT_CATEGORY_FIXTURE,
      .software_version_id = 1,
      .software_version_label = "FC-DMXv1",
      .queue_size_max = 32
    };
    dmx_personality_t personalities[] = {
        {10, "FC Personality"}
    };
    int personality_count = 1;

    Serial.println(F("Flux Capacitor DMX version " FC_VERSION " " FC_VERSION_EXTRA));
    
  
    // Start the DMX stuff
    dmx_driver_install(dmxPort, &config, personalities, personality_count);
    dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
}


/*********************************************************************************
 * 
 * loop
 *
 *********************************************************************************/

void dmx_loop() 
{
    
    if(dmx_receive(dmxPort, &packet, 0)) {
        
        lastDMXpacket = millis();
    
        if (!packet.err) {

            if(!dmxIsConnected) {
                Serial.println("DMX is connected!");
                dmxIsConnected = true;
            }
      
            dmx_read(dmxPort, data, packet.size);
      
            if(!data[0]) {
                setDisplay(FC_BASE);
            } else {
                Serial.printf("Unrecognized start code %d (0x%02x)", data[0]);
            }
          
        } else {
            
            Serial.println("A DMX error occurred.");
            
        }
        
    } 

    if(dmxIsConnected && (millis() - lastDMXpacket > 1250)) {
        Serial.println("DMX was disconnected.");
        dmxIsConnected = false;
    }
}



/*********************************************************************************
 * 
 * helpers
 *
 *********************************************************************************/


/*
  0 = ch1:  Master brightness (0-255) (scales down channels 2+3;
            chase lights off when master brightness == 0)
  1 = ch2:  Center LED (0-255) (0 = off, 255 brightest)
  2 = ch3:  Box LEDs   (0-255) (0 = off, 255 brightest)
  3 = ch4:  Chase 1 (on/off)                   > !
  4 = ch5:  Chase 2 (on/off)                   > ! Disregarded if ch10
  5 = ch6:  Chase 3 (on/off)                   > ! is non-zero ???
  6 = ch7:  Chase 4 (on/off)                   > ! 0-255; 0-127=off, 128-255=on
  7 = ch8:  Chase 5 (on/off)                   > !
  8 = ch9:  Chase 6 (on/off)                   > !

  Perhaps:
  9 = ch10: Chase Speed (1=slowest, 255=fastest; 0 = disabled, use ch4-ch9)         
*/

static void setDisplay(int base)
{
    int cbri, bbri, mbri;

    for(int i = 0; i < 6; i++) {
        data[base + 3 + i] >>= 7;
    }

    mbri = data[base + 0];
    
    if(data[base + 9]) {
        // Automatic chase
        if(mbri) {    // master bri
            fcLEDs.setSpeed( 257 - (uint16_t)data[base + 9] );
            fcLEDs.clearCurPattern();
        } else {
            fcLEDs.setCurPattern(0);
        }
    } else {
        // manual pattern selection
        uint8_t pat = 0;
        if(mbri) {    // master bri
            for(int i = 0; i < 6; i++) {
                pat <<= 1;
                pat |= (data[base + 3 + i] >> 7);
            }
        }
        fcLEDs.setCurPattern(pat);
    }

    cbri = bbri = 0;
    if(mbri) {    // master bri
        cbri = data[base + 1] * mbri / 255;
        bbri = data[base + 2] * mbri / 255;
    }
    centerLED.setDC(cbri);
    boxLED.setDC(bbri);
}


void showWaitSequence()
{
    fcLEDs.SpecialSignal(FCSEQ_WAIT);
}

void endWaitSequence()
{
    fcLEDs.SpecialSignal(0);
}

void showCopyError()
{
    fcLEDs.SpecialSignal(FCSEQ_ERRCOPY);
}

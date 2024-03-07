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

// The timer to use for the FC chase
#define FC_TIMER_NO   3    //  0 and 3 ok; 0 => group 0, num 0; 3 => group 1, num 1
// DMX uses 
//  group = dmx_port / 2;   port=1 -> 1/2 = 0 > group 0
//  num   = dmx_port % 2;   port=1 -> 1%2 = 1 > num   1

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
FCLEDs fcLEDs(FC_TIMER_NO, SHIFT_CLK_PIN, REG_CLK_PIN, SERDATA_PIN, MRESET_PIN);

int transmitPin = DMX_TRANSMIT;
int receivePin = DMX_RECEIVE;
int enablePin = DMX_ENABLE;

/* We have 3 ports (0-2). Port 0 is for the Serial Monitor. */
dmx_port_t dmxPort = 1;

dmx_packet_t packet;

uint8_t data[DMX_PACKET_SIZE];

#define DMX_ADDRESS   47
#define DMX_CHANNELS  10

#define DMX_VERIFY_CHANNEL 46    // must be set to DMX_VERIFY_VALUE
#define DMX_VERIFY_VALUE   100 

#if defined(DMX_USE_VERIFY) && (DMX_ADDRESS < DMX_VERIFY_CHANNEL)
#define DMX_SLOTS_TO_RECEIVE (DMX_VERIFY_CHANNEL + 1)
#else
#define DMX_SLOTS_TO_RECEIVE (DMX_ADDRESS + DMX_CHANNELS)
#endif

// DMX channels
#define FC_BASE DMX_ADDRESS

uint8_t cache[DMX_CHANNELS];

unsigned long powerupMillis;

static bool          dmxIsConnected = false;
static unsigned long lastDMXpacket;

static void setDisplay(int base);

static void invalidateCache()
{
    for(int i = 0; i < DMX_CHANNELS; i++) {
        cache[i] = rand() % 255;
    }
}

/*********************************************************************************
 * 
 * boot
 *
 *********************************************************************************/

void dmx_boot() 
{
    // Boot center LED here (is for some reason on after reset)
    centerLED.begin(CLED_CHANNEL, CLED_FREQ, CLED_RES);

    // Boot remaining display LEDs (but keep them dark)
    boxLED.begin(BLED_CHANNEL, BLED_FREQ, BLED_RES, 255);

    // Make sure LEDs are off
    centerLED.setDC(0);
    boxLED.setDC(0);

    // Do NOT boot fcLEDs here. Might disturb 
    // firmware update.

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
      .interrupt_flags = (DMX_INTR_FLAGS_DEFAULT | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_LEVEL2),
      .root_device_parameter_count = 32,
      .sub_device_parameter_count = 0,
      .model_id = 0,
      .product_category = RDM_PRODUCT_CATEGORY_FIXTURE,
      .software_version_id = 1,
      .software_version_label = "FC-DMXv1",
      .queue_size_max = 32
    };
    dmx_personality_t personalities[] = {
        {DMX_CHANNELS, "FC Personality"}
    };
    int personality_count = 1;

    // Boot FC leds
    fcLEDs.begin();

    Serial.println(F("Flux Capacitor DMX version " FC_VERSION " " FC_VERSION_EXTRA));
    Serial.println(F("(C) 2024 Thomas Winischhofer (A10001986)"));

    invalidateCache();

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
    #ifdef FC_DBG
    bool isAllZero = true;
    #endif
       
    if(dmx_receive_num(dmxPort, &packet, DMX_SLOTS_TO_RECEIVE, 0)) {
        
        lastDMXpacket = millis();
    
        if(!packet.err) {

            if(!dmxIsConnected) {
                Serial.println("DMX is connected");
                dmxIsConnected = true;
            }
      
            dmx_read(dmxPort, data, packet.size);
      
            if(!data[0]) {
              
                #ifdef FC_DBG1
                for(int i = DMX_ADDRESS; i < DMX_ADDRESS+DMX_CHANNELS; i++) {
                   if(data[i]) {
                        isAllZero = false;
                        break;
                   }
                }
                if(isAllZero) {
                    Serial.printf("Zero packet, size %d\n", packet.size);
                }
                #endif
              
                if(memcmp(cache, data + FC_BASE, DMX_CHANNELS)) {
                    setDisplay(FC_BASE);
                    memcpy(cache, data + FC_BASE, DMX_CHANNELS);
                }
                
            } else {
              
                Serial.printf("Unrecognized start code %d (0x%02x)", data[0], data[0]);
                
            }
          
        } else {
            
            Serial.printf("DMX error: %d\n", packet.err);
            
        }
        
    } 

    if(dmxIsConnected && (millis() - lastDMXpacket > 1250)) {
        Serial.println("DMX was disconnected");
        dmxIsConnected = false;
        invalidateCache();
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
  3 = ch4:  Auto chase (1=slowest, 255=fastest; 0=disabled, use ch5-ch10) 
  4 = ch5:  Chase 1 (on/off) (outer)           > !
  5 = ch6:  Chase 2 (on/off)                   > ! Disregarded if ch4
  6 = ch7:  Chase 3 (on/off)                   > ! is non-zero 
  7 = ch8:  Chase 4 (on/off)                   > ! 0-255; 0-127=off, 128-255=on
  8 = ch9:  Chase 5 (on/off)                   > !
  9 = ch10: Chase 6 (on/off) (inner)           > !
          
*/

static void setDisplay(int base)
{
    int cbri, bbri, mbri;

    mbri = data[base + 0];
    
    if(data[base + 3]) {
        // Automatic chase
        if(mbri) {
            // Speed: 255 = 2; 1 = 20; 0 = off
            fcLEDs.setSpeed( ((uint16_t)(255 - data[base + 3]) / 14) + 2);
            fcLEDs.clearCurPattern();
            fcLEDs.on();
        } else {
            fcLEDs.setCurPattern(0);
            fcLEDs.off();
        }
    } else {
        // manual pattern selection
        uint8_t pat = 0;
        if(mbri) {    // master bri
            for(int i = 0; i < 6; i++) {
                pat <<= 1;
                pat |= (data[base + 4 + i] >> 7);   // 0-127=off; 128-255=on
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
    //fcLEDs.SpecialSignal(FCSEQ_WAIT);     // No, fcLEDs not booted yet
    digitalWrite(IR_FB_PIN, HIGH);
}

void endWaitSequence()
{
    //fcLEDs.SpecialSignal(0);              // No, fcLEDs not booted yet
    digitalWrite(IR_FB_PIN, LOW);
}

void showCopyError()
{
    //fcLEDs.SpecialSignal(FCSEQ_ERRCOPY);  // No, fcLEDs not booted yet
    for(int i = 0; i < 10; i++) {
        digitalWrite(IR_FB_PIN, HIGH);
        delay(250);
        digitalWrite(IR_FB_PIN, LOW);
    }
}


# Firmware for Flux Capacitor - DMX controlled

This repository holds a firmware for CircuitSetup's Flux Capacitor which allows to control each element of the Flux capacitor through DMX. It is designed to work using the [Sparkfun LED-to-DMX](https://www.sparkfun.com/products/15110) shield.

(DMX control is also available for [Time Circuits Display](https://github.com/realA10001986/Time-Circuits-Display-DMX) and [SID](https://github.com/realA10001986/SID-DMX))

### DMX channels

<table>
    <tr><td>DMX channel</td><td>Function</td></tr>
    <tr><td>47</td><td>Master brightness (0-255)<br>(scales down channels 2+3; chase lights off when master brightness is 0)</td></tr>
    <tr><td>48</td><td>Center LED (0-255) (0=off, 255=brightest)</td></tr>
    <tr><td>49</td><td>Box LEDs   (0-255) (0=off, 255=brightest)</td></tr>
    <tr><td>50</td><td>Auto Chase (1=slowest, 255=fastest; 0=disabled, use ch51-ch56)</tr>
    <tr><td>51</td><td>Chase LED 1 (outer) (0-127=off, 128-255=on)</td></tr>
    <tr><td>52</td><td>Chase LED 2</td></tr>
    <tr><td>53</td><td>Chase LED 3</td></tr>
    <tr><td>54</td><td>Chase LED 4</td></tr>
    <tr><td>55</td><td>Chase LED 5</td></tr>
    <tr><td>56</td><td>Chase LED 6 (inner)</td></tr>
</table>

#### Packet verification

The DMX protocol uses no checksums. Therefore, transmission errors cannot be detected. Typically, such errors manifest themselves in flicker or flashing center or box lights. Since the Flux Capacitor is no ordinary light fixture, this can be an issue.

In order to at least filter out grossly malformed/corrupt DMX data packets, the firmware supports a simple DMX packet verifier: For a DMX data packet to be considered valid, _channel 46 must be at value 100_. If a packet contains any other value for this channel, the packet is ignored. 

To enable this filter, DMX_USE_VERIFY must be #defined in fc_global.h. This feature is disabled by default, because it hinders a global "black out". If your DMX controller can exclude channels from "black out" (or this function is not to be used), and you experience flicker, you can try to activate this packet verifier.

### Firmware update

To update the firmware without Arduino IDE/PlatformIO, copy a pre-compiled binary (filename must be "fcfw.bin") to a FAT32 formatted SD card, insert this card into the FC, and power up. The FC's IR feedback LED (little red light near the bright Center LED) will light up while the FC updates its firmware. Afterwards it will reboot.

### Build information

Requires [esp_dmx](https://github.com/someweisguy/esp_dmx) library v4.0.1 or later.

### Hardware: Pin mapping

<table>
    <tr>
     <td align="center">FC</td><td align="center">LED-to-DMX shield</td>
    </tr>
    <tr>
     <td align="center">TT IN (IO13)</a></td>
     <td align="center">J1 P14</td>
    </tr>
    <tr>
     <td align="center">IO14</td>
     <td align="center">J1 P15</td>
    </tr>
    <tr>
     <td align="center">Chase Speed D (IO32)</td>
     <td align="center">J1 P16</td>
    </tr>
  <tr>
     <td align="center">3V3</td>
     <td align="center">J1 P2</td>
    </tr>
  <tr>
     <td align="center">5V/GND from Chase Speed terminal</td>
     <td align="center">J12</td>
    </tr>
</table>

![DMXshield-FC](https://github.com/realA10001986/Flux-Capacitor-DMX/assets/76924199/42a24886-844a-4759-99c5-fcde2d7d13cb)



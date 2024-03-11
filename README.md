**&#9888; Not for public release**

# Firmware for Flux Capacitor - DMX controlled

This repository holds a firmware for CircuitSetup's Flux Capacitor which allows to control each element of the Flux capacitor through DMX. It is designed to work using the [Sparkfun LED-to-DMX](https://www.sparkfun.com/products/15110) shield.

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

If DMX_USE_VERIFY is defined in fc_global.h, a simple DMX packet verifier filters out malformed/corrupt DMX packets. For a packet to be considered valid, channel 46 must be at value 100. If this channel is set to any other value, the packet is ignored. This feature is disabled by default, because it hinders a global black out - unless your DMX controller can exclude channels from global black out.

### Build information

Requires [esp_dmx](https://github.com/someweisguy/esp_dmx) library v4.0.1 or later.

### Firmware update

To update the firmware without Arduino IDE/PlatformIO, copy a pre-compiled binary (filename must be "fcfw.bin") to a FAT32 formatted SD card, insert this card into the FC, and power up. The FC's IR feedback LED (little red light near the bright Center LED) will light up while the FC updates its firmware. Afterwards it will reboot.

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
     <td align="center">5V/GND from IO14 terminal</td>
     <td align="center">J12</td>
    </tr>
</table>

![DMXshield](https://github.com/realA10001986/Flux-Capacitor-DMX/assets/76924199/64e5bb4f-73d0-41de-8040-8dbd057bb981)


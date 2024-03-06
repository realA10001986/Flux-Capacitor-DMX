**&#9888; Not for public release**

# Firmware for Flux Capacitor - DMX controlled

This repository holds a firmware for CircuitSetup's Flux Capacitor which allows to control each element of the Flux capacitor through DMX. It is designed to work the the [Sparkfun LED-to-DMX](https://www.sparkfun.com/products/15110) shield.

### DMX channels

<table>
    <tr><td>DMX channel</td><td>Function</td></tr>
    <tr><td>36</td><td>Master brightness (0-255)<br>(scales down channels 2+3; chase lights off when master brightness is 0)</td></tr>
    <tr><td>37</td><td>Center LED (0-255) (0 = off, 255 brightest)</td></tr>
    <tr><td>38</td><td>Box LEDs   (0-255) (0 = off, 255 brightest)</td></tr>
    <tr><td>39</td><td>Chase LED 1 (outer) (0-127=off, 128-255=on)</td></tr>
    <tr><td>40</td><td>Chase LED 2</td></tr>
    <tr><td>41</td><td>Chase LED 3</td></tr>
    <tr><td>42</td><td>Chase LED 4</td></tr>
    <tr><td>43</td><td>Chase LED 5</td></tr>
    <tr><td>44</td><td>Chase LED 6 (inner)</td></tr>
    <tr><td>45</td><td>Auto Chase (1=slowest, 255=fastest; 0=disabled, use ch39-ch44)</tr>
</table>

### Build information

Requires "esp_dmx" library (someweisguy) v4.0.1 or later.

### Firmware update

To update the firmware without Arduino IDE/PlatformIO, copy a pre-compiled binary using the filename "fcfw.bin" to a FAT32 formatted SD card, insert this card into the FC, and power up. The FC's IR feedback LED (little red light near the bright Center LED) will light up while the FC updates its firmware. Afterward it will reboot.

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


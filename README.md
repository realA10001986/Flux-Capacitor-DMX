
# **&#9888; Not for public release**

Requires "esp_dmx" library (someweisguy) v4.0.1
(Ignore compiler warnings)

<table>
    <tr><td>36</td><td>Master brightness (0-255)<br>(scales down channels 2+3; chase lights off when master brightness is 0)</td></tr>
    <tr><td>37</td><td>Center LED (0-255) (0 = off, 255 brightest)</tr>
    <tr><td>38</td><td>Box LEDs   (0-255) (0 = off, 255 brightest)</tr>
    <tr><td>39</td><td>Chase 1 (on/off) (outer) </tr>
    <tr><td>40</td><td>Chase 2 (on/off)</tr>
    <tr><td>41</td><td>Chase 3 (on/off)</tr>
    <tr><td>42</td><td>Chase 4 (on/off)</tr>
    <tr><td>43</td><td>Chase 5 (on/off)</tr>
    <tr><td>44</td><td>Chase 6 (on/off) (inner)</tr>
    <tr><td>45</td><td>Auto Chase (1=slowest, 255=fastest; 0 = disabled, use ch39-ch44)</tr>
</table>

Pin mapping:
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


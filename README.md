# tx6u-arduino

#Introduction

Arduino implementation of La Crosse TX6U Remote Temperature Sensor with Arduino Uno, DHT22 temperature humidity sensor, 433Mhz Transmitter.
![]
(https://lh5.googleusercontent.com/-qIVOOzBeQVo/VWI0pGkRGnI/AAAAAAAAChM/Ct_1ZClvSMw/w1562-h1042-no/L26A2436.JPG)
#Purpose

Low cost remote temperature sensor comaptible with La Crosse Weather Station

#Parts

## Hardware
- Arduino Uno
- DHT22 Temperature Humidity Sensor
- 433Mhz Transmitter
- 10k resistor
- Breadboard

## Software
- Adafruit's [DHT library](https://learn.adafruit.com/dht?view=all)

#Wiring

#Testing
<img src="https://lh3.googleusercontent.com/-VZ71K9ljQiw/VWI7aLff35I/AAAAAAAACio/orYc6GPfWn0/w844-h1358-no/L26A2455.JPG" height="500">

## La Crosse Weather Station WS-9016U

WS-9016U consists of Indoor Weather Station(WS-9016U) and Remote Temperature Sensor(TX6U)

Acording to the manual, 

- Power up remote sensor first, then, power up base station
- Range of 80 feet
- 
### WS-9016U Indoor Weather Station,

- can have up to 3 remote sensors
- receive outdoor temperature every 5 minute

### TX6U Remote Temperature Sensor

- check temperature every 1 minute
- transmit a signal every 3 minutes
 
#License and Credits

tx6u-arduino is based on Adafruit's [DHT library](https://learn.adafruit.com/dht?view=all) and jremington's [TX6U_send.ino](http://forum.arduino.cc/index.php?topic=303888.0) source code.

tx6u-arduino is distributed under GPL License.

#Reference

- http://forum.arduino.cc/index.php?topic=303888.0
- https://learn.adafruit.com/dht?view=all
- http://shop.ninjablocks.com/blogs/how-to/7501042-adding-rf-433mhz

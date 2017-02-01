# mAteo

mAteo is a small and simple weather station build on Arduino Nano.

It implements measures of temperature, humidity and pressure using DH22 (tempetaure and umidity) and BMP180 (pressure) sensors modules.
It also implements a clock build with "1 wire" approach (
[inspired from here](http://www.instructables.com/id/Make-an-accurate-Arduino-clock-using-only-one-wire/)).
Data are shown on a 0.96" SSD1306 OLED display.
The user can interact with a 5 keys keyboard, build on the basis of Arduino starter kit tutorial, that use only 1 analog input.

Real time data are shown on screen all together.
Graphs of last 24hrs (1 point each 30') are on separated screens.
Data for graph are stored in EEPROM.

## BOM
- Arduino NANO
- DHT22 temperature and humidity sensor
- BMP180 temparture and pressure sensor
- SSD1306 0.96" OLED display
- 5 pushbuttons
- 1 220ohm resistor
- 1 2kohm resistor
- 1 4.7kohm resistor
- 2 10kohm resistors

## Arduino pin used
Single wire clock
- D2 interrupt
- D3 PWM used for clock

keyboard
- A6

I2C
- A4 SDA
- A5 SCL

Power supply of components
- 5V
- GND

Arduino NANO is powerd with onboard USB.

## SW dependencies
- Wire
- SFE_BMP180
- DHT *
- Adafruid_GFX *
- Adafruid_SSD1306 *
- EEPROM

(*) these have been modified to reduce memory footprint and have not been uploaded yet on my github.

## how to use
chapter to be witten




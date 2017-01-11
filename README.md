# mAteo

Developped (well... actually under development) on Arduino NANO.

mAteo is a small "weather station" that measures current indoor temperature and humidity (with a DHT22) and pressure (witth BMP180) and display data on a 0.96" SSD1306 OLED display.
Also implements clock without using RTC but with the "1 wire" approach that uses pwm and external interrupt.
(inspired by http://www.instructables.com/id/Make-an-accurate-Arduino-clock-using-only-one-wire/)
Clock is preset at sketch compiling date and time, manages leap year and it's adjustable by end-user.
The 5 keys keyboard is analogic and designed according te example in Arduino official book of the started kit.

*** From now on the functions are not yet implemented ***
Also displays graphics of data trends (temperature, humidity and pressure) in the last 24hrs.
To be evaluated the possibility to monitor also data from remote sensors to be connected wireless HC-12 (needs a sencond Arduino).
(http://www.instructables.com/id/Long-Range-18km-Arduino-to-Arduino-Wireless-Commun/)

Alternative keyboard approach to be evaluated (it looks to be more simple)
(http://tronixstuff.com/2011/01/11/tutorial-using-analog-input-for-multiple-buttons/)

*** BOM ***
- Arduino NANO			1
- DHT22 sensor module	1
- BMP180 sensor module	1
- SSD1306				1
- push buttons			5
- 9V battery			1
- 9V battery clip		1
- resistors				
- resistors				
- resistors				
- resistors				
- resistors	10M			2

Arduino connections:
DHT					D12
keyboard			A6
battery monitor		A7
PWM (clock)			D3
Interrupt (clock)	D2
SDA					A4
SCL					A5
VIN					9V power supply
5V					to all periferals

schematics to be done.
EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:w_analog
LIBS:w_microcontrollers
LIBS:w_transistor
LIBS:w_connectors
LIBS:w_memory
LIBS:w_rtx
LIBS:w_logic
LIBS:w_opto
LIBS:w_vacuum
LIBS:w_device
LIBS:w_relay
LIBS:mAteo-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "mAteo - la stazione meteo di Matteo"
Date "2017-01-27"
Rev "0.1"
Comp "matteomarchesi"
Comment1 "https://github.com/matteomarchesi/mAteo"
Comment2 "https://matteomarchesi.altervista.org"
Comment3 "matteo.marchesi71@gmail.com"
Comment4 ""
$EndDescr
$Comp
L R R1
U 1 1 588BE974
P 2550 2800
F 0 "R1" V 2630 2800 50  0000 C CNN
F 1 "220" V 2550 2800 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 2480 2800 50  0001 C CNN
F 3 "" H 2550 2800 50  0000 C CNN
	1    2550 2800
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 588BE9D2
P 2900 2800
F 0 "R2" V 2980 2800 50  0000 C CNN
F 1 "1k" V 2900 2800 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 2830 2800 50  0001 C CNN
F 3 "" H 2900 2800 50  0000 C CNN
	1    2900 2800
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 588BEA8F
P 3300 2800
F 0 "R3" V 3380 2800 50  0000 C CNN
F 1 "4.7k" V 3300 2800 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 3230 2800 50  0001 C CNN
F 3 "" H 3300 2800 50  0000 C CNN
	1    3300 2800
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 588BEAC9
P 3700 2800
F 0 "R4" V 3780 2800 50  0000 C CNN
F 1 "10k" V 3700 2800 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 3630 2800 50  0001 C CNN
F 3 "" H 3700 2800 50  0000 C CNN
	1    3700 2800
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 588BEB3B
P 3700 3800
F 0 "R5" V 3780 3800 50  0000 C CNN
F 1 "10k" V 3700 3800 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 3630 3800 50  0001 C CNN
F 3 "" H 3700 3800 50  0000 C CNN
	1    3700 3800
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 588BEBEC
P 2550 3300
F 0 "SW2" H 2560 3420 50  0000 C CNN
F 1 "SW_PUSH" H 2550 3220 50  0000 C CNN
F 2 "w_switch:PCB_PUSH" H 2550 3300 60  0001 C CNN
F 3 "" H 2550 3300 60  0000 C CNN
	1    2550 3300
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 588BEF00
P 2100 3300
F 0 "SW1" H 2110 3420 50  0000 C CNN
F 1 "SW_PUSH" H 2100 3220 50  0000 C CNN
F 2 "w_switch:PCB_PUSH" H 2100 3300 60  0001 C CNN
F 3 "" H 2100 3300 60  0000 C CNN
	1    2100 3300
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW3
U 1 1 588BEF58
P 2900 3300
F 0 "SW3" H 2910 3420 50  0000 C CNN
F 1 "SW_PUSH" H 2900 3220 50  0000 C CNN
F 2 "w_switch:PCB_PUSH" H 2900 3300 60  0001 C CNN
F 3 "" H 2900 3300 60  0000 C CNN
	1    2900 3300
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW4
U 1 1 588BEF90
P 3300 3300
F 0 "SW4" H 3310 3420 50  0000 C CNN
F 1 "SW_PUSH" H 3300 3220 50  0000 C CNN
F 2 "w_switch:PCB_PUSH" H 3300 3300 60  0001 C CNN
F 3 "" H 3300 3300 60  0000 C CNN
	1    3300 3300
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW5
U 1 1 588BEFCD
P 3700 3300
F 0 "SW5" H 3710 3420 50  0000 C CNN
F 1 "SW_PUSH" H 3700 3220 50  0000 C CNN
F 2 "w_switch:PCB_PUSH" H 3700 3300 60  0001 C CNN
F 3 "" H 3700 3300 60  0000 C CNN
	1    3700 3300
	0    1    1    0   
$EndComp
Text Notes 2100 1900 0    60   ~ 0
Keyboard
$Comp
L Arduino_Nano_Header J1
U 1 1 588BFD70
P 6200 3050
F 0 "J1" H 6200 3850 60  0000 C CNN
F 1 "Arduino_Nano_Header" H 6200 2250 60  0000 C CNN
F 2 "w_conn_misc:arduino_nano_header" H 6200 3050 60  0001 C CNN
F 3 "" H 6200 3050 60  0000 C CNN
	1    6200 3050
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P1
U 1 1 588C1241
P 8200 2450
F 0 "P1" H 8200 2650 50  0000 C CNN
F 1 "CONN_01X03" V 8300 2450 50  0000 C CNN
F 2 "w_pin_strip:pin_strip_3" H 8200 2450 50  0001 C CNN
F 3 "" H 8200 2450 50  0000 C CNN
	1    8200 2450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P2
U 1 1 588C169C
P 8200 3000
F 0 "P2" H 8200 3250 50  0000 C CNN
F 1 "CONN_01X04" V 8300 3000 50  0000 C CNN
F 2 "w_pin_strip:pin_strip_4" H 8200 3000 50  0001 C CNN
F 3 "" H 8200 3000 50  0000 C CNN
	1    8200 3000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P3
U 1 1 588C16F7
P 8200 3650
F 0 "P3" H 8200 3900 50  0000 C CNN
F 1 "CONN_01X04" V 8300 3650 50  0000 C CNN
F 2 "w_pin_strip:pin_strip_4" H 8200 3650 50  0001 C CNN
F 3 "" H 8200 3650 50  0000 C CNN
	1    8200 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3450 3700 3650
Wire Wire Line
	3700 3150 3700 2950
Wire Wire Line
	3300 3150 3300 2950
Wire Wire Line
	2900 3150 2900 2950
Wire Wire Line
	2550 3150 2550 2950
Wire Wire Line
	2100 3150 2100 2550
Wire Wire Line
	2100 2550 3700 2550
Connection ~ 3700 2550
Wire Wire Line
	3300 2650 3300 2550
Connection ~ 3300 2550
Wire Wire Line
	2900 2650 2900 2550
Connection ~ 2900 2550
Wire Wire Line
	2550 2650 2550 2550
Connection ~ 2550 2550
Wire Wire Line
	2100 3550 4000 3550
Wire Wire Line
	2100 3550 2100 3450
Connection ~ 3700 3550
Wire Wire Line
	2550 3450 2550 3550
Connection ~ 2550 3550
Wire Wire Line
	2900 3450 2900 3550
Connection ~ 2900 3550
Wire Wire Line
	3300 3450 3300 3550
Connection ~ 3300 3550
Wire Notes Line
	1800 2350 1800 4300
Wire Notes Line
	1800 4300 4800 4300
Wire Notes Line
	4800 4300 4800 1600
Wire Notes Line
	4800 1600 1800 1600
Wire Notes Line
	1800 1600 1800 2400
Wire Wire Line
	3700 4050 3700 3950
Wire Wire Line
	7200 2150 7200 4050
Wire Wire Line
	5850 2650 5550 2650
Wire Wire Line
	5550 2650 5550 2150
Connection ~ 7200 2450
Wire Wire Line
	7200 4050 3700 4050
Wire Wire Line
	6550 2650 7950 2650
Wire Wire Line
	3700 1700 3700 2650
Wire Wire Line
	6550 2850 7100 2850
Wire Wire Line
	7100 2850 7100 3950
Wire Wire Line
	7100 3950 4000 3950
Wire Wire Line
	4000 3950 4000 3550
Wire Wire Line
	7650 2550 8000 2550
Connection ~ 7650 2550
Wire Wire Line
	7950 3500 8000 3500
Wire Wire Line
	7950 2850 8000 2850
Connection ~ 7950 2850
Wire Wire Line
	7650 2450 7650 3800
Wire Wire Line
	7850 3600 7850 2950
Connection ~ 7850 2950
Wire Wire Line
	7750 3700 7750 3050
Connection ~ 7750 3050
Wire Wire Line
	5550 2150 7200 2150
Wire Wire Line
	7650 3800 8000 3800
Wire Wire Line
	8000 3700 7750 3700
Wire Wire Line
	8000 3600 7850 3600
Wire Wire Line
	7950 1700 7950 3500
Wire Wire Line
	7950 2350 8000 2350
Connection ~ 7950 2650
Wire Wire Line
	8000 3150 7650 3150
Connection ~ 7650 3150
Wire Wire Line
	8000 2450 7750 2450
Wire Wire Line
	7750 2450 7750 2000
Wire Wire Line
	7750 2000 5700 2000
Wire Wire Line
	5700 2000 5700 3450
Wire Wire Line
	5700 3450 5850 3450
Text Notes 8000 2050 0    60   ~ 0
Sensors connectors
Text Notes 8550 2400 0    60   ~ 0
DHR22
Text Notes 8600 2950 0    60   ~ 0
BMP180
Text Notes 8600 3600 0    60   ~ 0
OLED display
Wire Wire Line
	5850 2750 5850 2850
Connection ~ 7950 2350
Wire Wire Line
	7950 1700 3700 1700
Wire Wire Line
	6550 2950 8000 2950
Wire Wire Line
	6550 3050 8000 3050
Wire Wire Line
	6550 2450 7650 2450
$EndSCHEMATC

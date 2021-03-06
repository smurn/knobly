EESchema Schematic File Version 2
LIBS:knobly-rescue
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
LIBS:pic18f4550_tqfp
LIBS:switch-enc
LIBS:encoder
LIBS:knobly-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "Knobly - Controller"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PIC18F4550_TQFP U1
U 1 1 575EBC67
P 4400 3850
F 0 "U1" H 5950 1750 50  0000 C CNN
F 1 "PIC18F4550_TQFP" H 4400 3850 50  0000 C CNN
F 2 "Housings_QFP:TQFP-44_10x10mm_Pitch0.8mm" H 4400 3850 60  0001 C CNN
F 3 "" H 4400 3850 60  0000 C CNN
	1    4400 3850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR01
U 1 1 575EBDB4
P 6850 4050
F 0 "#PWR01" H 6850 3900 50  0001 C CNN
F 1 "VCC" H 6850 4200 50  0000 C CNN
F 2 "" H 6850 4050 50  0000 C CNN
F 3 "" H 6850 4050 50  0000 C CNN
	1    6850 4050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 575EBE2E
P 7050 3750
F 0 "#PWR02" H 7050 3500 50  0001 C CNN
F 1 "GND" H 7050 3600 50  0000 C CNN
F 2 "" H 7050 3750 50  0000 C CNN
F 3 "" H 7050 3750 50  0000 C CNN
	1    7050 3750
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR03
U 1 1 575EBE84
P 1850 3950
F 0 "#PWR03" H 1850 3800 50  0001 C CNN
F 1 "VCC" H 1850 4100 50  0000 C CNN
F 2 "" H 1850 3950 50  0000 C CNN
F 3 "" H 1850 3950 50  0000 C CNN
	1    1850 3950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR04
U 1 1 575EBE9C
P 1850 3650
F 0 "#PWR04" H 1850 3400 50  0001 C CNN
F 1 "GND" H 1850 3500 50  0000 C CNN
F 2 "" H 1850 3650 50  0000 C CNN
F 3 "" H 1850 3650 50  0000 C CNN
	1    1850 3650
	-1   0    0    1   
$EndComp
Text GLabel 2500 3450 0    60   Input ~ 0
COL9
Text GLabel 2500 3550 0    60   Input ~ 0
COL11
Text GLabel 2500 3650 0    60   Input ~ 0
COL13
Text GLabel 2500 3750 0    60   Input ~ 0
COL15
Text GLabel 4650 1950 1    60   Input ~ 0
COL1
Text GLabel 4400 1950 1    60   Input ~ 0
COL3
Text GLabel 4150 1950 1    60   Input ~ 0
COL5
Text GLabel 3900 1950 1    60   Input ~ 0
COL7
$Comp
L C C1
U 1 1 575EC054
P 5050 1500
F 0 "C1" H 5075 1600 50  0000 L CNN
F 1 "470n" H 5075 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5088 1350 50  0001 C CNN
F 3 "" H 5050 1500 50  0000 C CNN
	1    5050 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 575EC0F3
P 5050 1650
F 0 "#PWR05" H 5050 1400 50  0001 C CNN
F 1 "GND" H 5050 1500 50  0000 C CNN
F 2 "" H 5050 1650 50  0000 C CNN
F 3 "" H 5050 1650 50  0000 C CNN
	1    5050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1650 5050 1650
Wire Wire Line
	5050 1350 5050 1300
Wire Wire Line
	5050 1300 4900 1300
Wire Wire Line
	4900 1300 4900 1950
$Comp
L USB_B P1
U 1 1 575EC303
P 2900 1350
F 0 "P1" H 3100 1150 50  0000 C CNN
F 1 "USB_B" H 2850 1550 50  0000 C CNN
F 2 "mystuff:USBBVertical" V 2850 1250 50  0001 C CNN
F 3 "" V 2850 1250 50  0000 C CNN
	1    2900 1350
	0    -1   1    0   
$EndComp
Wire Wire Line
	3200 1350 3400 1350
Wire Wire Line
	3400 1350 3400 1950
Wire Wire Line
	3200 1250 3650 1250
Wire Wire Line
	3650 1250 3650 1950
$Comp
L GND #PWR06
U 1 1 575EC6FE
P 3250 1700
F 0 "#PWR06" H 3250 1450 50  0001 C CNN
F 1 "GND" H 3250 1550 50  0000 C CNN
F 2 "" H 3250 1700 50  0000 C CNN
F 3 "" H 3250 1700 50  0000 C CNN
	1    3250 1700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR07
U 1 1 575EC718
P 3250 1100
F 0 "#PWR07" H 3250 950 50  0001 C CNN
F 1 "VCC" H 3250 1250 50  0000 C CNN
F 2 "" H 3250 1100 50  0000 C CNN
F 3 "" H 3250 1100 50  0000 C CNN
	1    3250 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1150 3250 1150
Wire Wire Line
	3250 1150 3250 1100
Wire Wire Line
	3200 1450 3250 1450
Wire Wire Line
	3250 1450 3250 1700
Wire Wire Line
	2800 1650 3250 1650
Connection ~ 3250 1650
$Comp
L RJ12 J1
U 1 1 575EC8C0
P 3300 6950
F 0 "J1" H 3500 7450 50  0000 C CNN
F 1 "RJ12" H 3150 7450 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06" H 3300 6950 50  0001 C CNN
F 3 "" H 3300 6950 50  0000 C CNN
	1    3300 6950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3750 7150 5000 7150
Wire Wire Line
	4650 7150 4650 6000
$Comp
L VCC #PWR08
U 1 1 575ECD76
P 3800 7050
F 0 "#PWR08" H 3800 6900 50  0001 C CNN
F 1 "VCC" H 3800 7200 50  0000 C CNN
F 2 "" H 3800 7050 50  0000 C CNN
F 3 "" H 3800 7050 50  0000 C CNN
	1    3800 7050
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 575ECD92
P 4150 6950
F 0 "#PWR09" H 4150 6700 50  0001 C CNN
F 1 "GND" H 4150 6800 50  0000 C CNN
F 2 "" H 4150 6950 50  0000 C CNN
F 3 "" H 4150 6950 50  0000 C CNN
	1    4150 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 7050 3800 7050
Wire Wire Line
	4150 6950 3750 6950
NoConn ~ 3750 6650
$Comp
L GND #PWR010
U 1 1 575ED12F
P 3150 6400
F 0 "#PWR010" H 3150 6150 50  0001 C CNN
F 1 "GND" H 3150 6250 50  0000 C CNN
F 2 "" H 3150 6400 50  0000 C CNN
F 3 "" H 3150 6400 50  0000 C CNN
	1    3150 6400
	1    0    0    -1  
$EndComp
Text GLabel 4900 6000 3    60   Input ~ 0
ROW0
Text GLabel 5150 6000 3    60   Input ~ 0
ROW1
Text GLabel 5400 6000 3    60   Input ~ 0
ROW2
Text GLabel 5650 6000 3    60   Input ~ 0
ROW3
Text GLabel 6300 4350 2    60   Input ~ 0
ROW4
Text GLabel 6300 4250 2    60   Input ~ 0
ROW5
Text GLabel 2500 4050 0    60   Input ~ 0
COL0
Text GLabel 2500 4150 0    60   Input ~ 0
COL2
Text GLabel 2500 4250 0    60   Input ~ 0
COL4
Text GLabel 2500 4350 0    60   Input ~ 0
COL6
Text GLabel 3650 6000 3    60   Input ~ 0
COL8
Text GLabel 3900 6000 3    60   Input ~ 0
COL10
NoConn ~ 3150 6000
NoConn ~ 3400 6000
NoConn ~ 5650 1950
$Comp
L Crystal Y1
U 1 1 575EE04A
P 7750 3600
F 0 "Y1" H 7750 3750 50  0000 C CNN
F 1 "20Mhz" H 7750 3450 50  0000 C CNN
F 2 "mystuff:HC49US" H 7750 3600 50  0001 C CNN
F 3 "" H 7750 3600 50  0000 C CNN
	1    7750 3600
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 575EE652
P 8050 3450
F 0 "C2" H 8075 3550 50  0000 L CNN
F 1 "15p" H 8075 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8088 3300 50  0001 C CNN
F 3 "" H 8050 3450 50  0000 C CNN
	1    8050 3450
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 575EE6AB
P 8050 3750
F 0 "C3" H 8075 3850 50  0000 L CNN
F 1 "15p" H 8075 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8088 3600 50  0001 C CNN
F 3 "" H 8050 3750 50  0000 C CNN
	1    8050 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 575EE6E4
P 8300 3700
F 0 "#PWR011" H 8300 3450 50  0001 C CNN
F 1 "GND" H 8300 3550 50  0000 C CNN
F 2 "" H 8300 3700 50  0000 C CNN
F 3 "" H 8300 3700 50  0000 C CNN
	1    8300 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 3700 8300 3600
Wire Wire Line
	8300 3600 8050 3600
Wire Wire Line
	7750 3450 7750 3250
Wire Wire Line
	7450 3250 8050 3250
Wire Wire Line
	8050 3250 8050 3300
Wire Wire Line
	7750 3750 7750 3950
Wire Wire Line
	7450 3950 8050 3950
Wire Wire Line
	8050 3950 8050 3900
Wire Wire Line
	6300 3550 7450 3550
Wire Wire Line
	7450 3550 7450 3250
Connection ~ 7750 3250
Wire Wire Line
	6300 3650 7450 3650
Wire Wire Line
	7450 3650 7450 3950
Connection ~ 7750 3950
Wire Wire Line
	3150 6350 3150 6400
Wire Wire Line
	2900 6350 3150 6350
Wire Wire Line
	3000 6350 3000 6450
Wire Wire Line
	2900 6350 2900 6450
Connection ~ 3000 6350
$Comp
L C C4
U 1 1 575EFB9F
P 6850 3900
F 0 "C4" H 6875 4000 50  0000 L CNN
F 1 "100n" H 6875 3800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6888 3750 50  0001 C CNN
F 3 "" H 6850 3900 50  0000 C CNN
	1    6850 3900
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 575F0003
P 1850 3800
F 0 "C5" H 1875 3900 50  0000 L CNN
F 1 "100n" H 1875 3700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1888 3650 50  0001 C CNN
F 3 "" H 1850 3800 50  0000 C CNN
	1    1850 3800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR012
U 1 1 575F0371
P 5000 6650
F 0 "#PWR012" H 5000 6500 50  0001 C CNN
F 1 "VCC" H 5000 6800 50  0000 C CNN
F 2 "" H 5000 6650 50  0000 C CNN
F 3 "" H 5000 6650 50  0000 C CNN
	1    5000 6650
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 575F03A9
P 5000 6900
F 0 "R1" V 5080 6900 50  0000 C CNN
F 1 "50k" V 5000 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4930 6900 50  0001 C CNN
F 3 "" H 5000 6900 50  0000 C CNN
	1    5000 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 7150 5000 7050
Connection ~ 4650 7150
Wire Wire Line
	5000 6750 5000 6650
NoConn ~ 6300 3350
NoConn ~ 2500 3350
NoConn ~ 3150 1950
Wire Wire Line
	6650 7500 6650 7200
$Comp
L GND #PWR013
U 1 1 575EBF2E
P 6650 7500
F 0 "#PWR013" H 6650 7250 50  0001 C CNN
F 1 "GND" H 6650 7350 50  0000 C CNN
F 2 "" H 6650 7500 50  0000 C CNN
F 3 "" H 6650 7500 50  0000 C CNN
	1    6650 7500
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG014
U 1 1 575EBF18
P 6650 7200
F 0 "#FLG014" H 6650 7295 50  0001 C CNN
F 1 "PWR_FLAG" H 6650 7380 50  0000 C CNN
F 2 "" H 6650 7200 50  0000 C CNN
F 3 "" H 6650 7200 50  0000 C CNN
	1    6650 7200
	1    0    0    -1  
$EndComp
Text GLabel 4150 6100 3    60   Input ~ 0
COL12
Text GLabel 4400 6100 3    60   Input ~ 0
COL14
Wire Wire Line
	4150 6000 4150 6100
Wire Wire Line
	4400 6000 4400 6100
Wire Wire Line
	4150 6050 4050 6050
Wire Wire Line
	4050 6050 4050 6750
Connection ~ 4150 6050
Wire Wire Line
	4400 6050 4300 6050
Wire Wire Line
	4300 6050 4300 6850
Wire Wire Line
	4300 6850 3750 6850
Connection ~ 4400 6050
Wire Wire Line
	4050 6750 3750 6750
NoConn ~ 6300 3450
Text GLabel 5400 1950 1    60   Input ~ 0
ROW6
Text GLabel 6300 4150 2    60   Input ~ 0
ENC0
Text GLabel 6300 4050 2    60   Input ~ 0
ENC1
Text GLabel 6300 3950 2    60   Input ~ 0
ENC2
NoConn ~ 5150 1950
$Comp
L VCC #PWR015
U 1 1 576ECAEA
P 6150 7200
F 0 "#PWR015" H 6150 7050 50  0001 C CNN
F 1 "VCC" H 6150 7350 50  0000 C CNN
F 2 "" H 6150 7200 50  0000 C CNN
F 3 "" H 6150 7200 50  0000 C CNN
	1    6150 7200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 576ECB13
P 6150 7500
F 0 "#PWR016" H 6150 7250 50  0001 C CNN
F 1 "GND" H 6150 7350 50  0000 C CNN
F 2 "" H 6150 7500 50  0000 C CNN
F 3 "" H 6150 7500 50  0000 C CNN
	1    6150 7500
	1    0    0    -1  
$EndComp
$Comp
L CP C6
U 1 1 576ECC87
P 6150 7350
F 0 "C6" H 6175 7450 50  0000 L CNN
F 1 "10u" H 6175 7250 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x4.5" H 6188 7200 50  0001 C CNN
F 3 "" H 6150 7350 50  0000 C CNN
	1    6150 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3950 1850 3950
Wire Wire Line
	1850 3650 2050 3650
Wire Wire Line
	2050 3650 2050 3850
Wire Wire Line
	2050 3850 2500 3850
Wire Wire Line
	6300 3750 7050 3750
Connection ~ 6850 3750
Wire Wire Line
	6300 3850 6700 3850
Wire Wire Line
	6700 3850 6700 4050
Wire Wire Line
	6700 4050 6850 4050
$EndSCHEMATC

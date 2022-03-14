EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L OpenBCI_ESP32:TPD4E1B06 U1
U 1 1 6197F390
P 2200 3350
F 0 "U1" V 2488 3446 50  0000 L CNN
F 1 "TPD4E1B06" V 1850 3200 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 2500 3350 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 2325 3675 50  0001 C CNN
	1    2200 3350
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x13_Odd_Even J1
U 1 1 61980577
P 3300 1900
F 0 "J1" H 3350 2717 50  0000 C CNN
F 1 "Conn_02x13_Odd_Even" H 3350 2626 50  0000 C CNN
F 2 "OpenBCI_ESP32:PinHeader_2x13_P2.54mm_OpenBCI" H 3300 1900 50  0001 C CNN
F 3 "~" H 3300 1900 50  0001 C CNN
	1    3300 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1300 2750 1300
Wire Wire Line
	3100 1600 2600 1600
Wire Wire Line
	3100 1700 2600 1700
Wire Wire Line
	3100 1800 2600 1800
Wire Wire Line
	3100 1900 2600 1900
Wire Wire Line
	3100 2000 2600 2000
Wire Wire Line
	3100 2100 2600 2100
Wire Wire Line
	3100 2200 2600 2200
Wire Wire Line
	3100 2300 2600 2300
Wire Wire Line
	3100 2400 2600 2400
Wire Wire Line
	3600 1300 3800 1300
Wire Wire Line
	3600 1600 4100 1600
Wire Wire Line
	3600 1700 4100 1700
Wire Wire Line
	3600 1800 4100 1800
Wire Wire Line
	3600 1900 4100 1900
Wire Wire Line
	3600 2000 4100 2000
Wire Wire Line
	3600 2100 4100 2100
Wire Wire Line
	3600 2200 4100 2200
Wire Wire Line
	3600 2300 4100 2300
Wire Wire Line
	3600 2400 4100 2400
$Comp
L power:+2V5 #PWR04
U 1 1 61ABC532
P 2750 1050
F 0 "#PWR04" H 2750 900 50  0001 C CNN
F 1 "+2V5" H 2765 1223 50  0000 C CNN
F 2 "" H 2750 1050 50  0001 C CNN
F 3 "" H 2750 1050 50  0001 C CNN
	1    2750 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1050 2750 1150
Wire Wire Line
	2750 1150 3800 1150
Wire Wire Line
	3800 1150 3800 1300
Connection ~ 2750 1150
Wire Wire Line
	2750 1150 2750 1300
$Comp
L power:GND #PWR01
U 1 1 61ABF2F6
P 2400 1150
F 0 "#PWR01" H 2400 900 50  0001 C CNN
F 1 "GND" H 2405 977 50  0000 C CNN
F 2 "" H 2400 1150 50  0001 C CNN
F 3 "" H 2400 1150 50  0001 C CNN
	1    2400 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 61ABFCA4
P 4150 1200
F 0 "#PWR06" H 4150 950 50  0001 C CNN
F 1 "GND" H 4155 1027 50  0000 C CNN
F 2 "" H 4150 1200 50  0001 C CNN
F 3 "" H 4150 1200 50  0001 C CNN
	1    4150 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1100 4150 1200
Wire Wire Line
	2600 1100 2400 1100
Wire Wire Line
	2400 1100 2400 1150
Wire Wire Line
	2600 1100 2600 1400
Wire Wire Line
	2600 1400 3100 1400
Wire Wire Line
	3600 1400 3950 1400
Wire Wire Line
	3950 1400 3950 1100
Wire Wire Line
	3950 1100 4150 1100
Wire Wire Line
	3600 1500 4100 1500
Wire Wire Line
	3100 1500 2600 1500
Text Label 2600 1500 0    50   ~ 0
BIAS
Text Label 2600 1600 0    50   ~ 0
8N
Text Label 2600 1700 0    50   ~ 0
7N
Text Label 2600 1800 0    50   ~ 0
6N
Text Label 2600 1900 0    50   ~ 0
5N
Text Label 2600 2000 0    50   ~ 0
4N
Text Label 2600 2100 0    50   ~ 0
3N
Text Label 2600 2200 0    50   ~ 0
2N
Text Label 2600 2300 0    50   ~ 0
1N
Text Label 2600 2400 0    50   ~ 0
SRB2
$Comp
L power:-2V5 #PWR02
U 1 1 61AC6C17
P 2400 2400
F 0 "#PWR02" H 2400 2500 50  0001 C CNN
F 1 "-2V5" H 2415 2573 50  0000 C CNN
F 2 "" H 2400 2400 50  0001 C CNN
F 3 "" H 2400 2400 50  0001 C CNN
	1    2400 2400
	1    0    0    -1  
$EndComp
$Comp
L power:-2V5 #PWR07
U 1 1 61AC74A1
P 4300 2400
F 0 "#PWR07" H 4300 2500 50  0001 C CNN
F 1 "-2V5" H 4315 2573 50  0000 C CNN
F 2 "" H 4300 2400 50  0001 C CNN
F 3 "" H 4300 2400 50  0001 C CNN
	1    4300 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2500 4300 2400
Wire Wire Line
	3600 2500 4300 2500
Wire Wire Line
	2400 2400 2400 2500
Wire Wire Line
	2400 2500 3100 2500
Text Label 4100 1500 2    50   ~ 0
BIAS
Text Label 4100 1600 2    50   ~ 0
8P
Text Label 4100 1700 2    50   ~ 0
7P
Text Label 4100 1800 2    50   ~ 0
6P
Text Label 4100 1900 2    50   ~ 0
5P
Text Label 4100 2000 2    50   ~ 0
4P
Text Label 4100 2100 2    50   ~ 0
3P
Text Label 4100 2200 2    50   ~ 0
2P
Text Label 4100 2300 2    50   ~ 0
1P
Text Label 4100 2400 2    50   ~ 0
SRB1
$Comp
L Device:R R1
U 1 1 61ACED98
P 6600 1550
F 0 "R1" V 6393 1550 50  0000 C CNN
F 1 "2k2" V 6484 1550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 1550 50  0001 C CNN
F 3 "~" H 6600 1550 50  0001 C CNN
	1    6600 1550
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 61ACFE88
P 6600 2050
F 0 "R2" V 6393 2050 50  0000 C CNN
F 1 "2k2" V 6484 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 2050 50  0001 C CNN
F 3 "~" H 6600 2050 50  0001 C CNN
	1    6600 2050
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 61AD0B0A
P 7100 1400
F 0 "C1" V 6848 1400 50  0000 C CNN
F 1 "1n" V 6939 1400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 1250 50  0001 C CNN
F 3 "~" H 7100 1400 50  0001 C CNN
	1    7100 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 1550 7400 1550
Wire Wire Line
	7250 1400 7400 1400
Wire Wire Line
	7400 1400 7400 1550
Connection ~ 7400 1550
Wire Wire Line
	7400 1550 7800 1550
Wire Wire Line
	6950 1400 6850 1400
Wire Wire Line
	6850 1400 6850 1900
$Comp
L Device:C C2
U 1 1 61AD5946
P 7100 1900
F 0 "C2" V 6848 1900 50  0000 C CNN
F 1 "1n" V 6939 1900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 1750 50  0001 C CNN
F 3 "~" H 7100 1900 50  0001 C CNN
	1    7100 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 2050 7400 2050
Wire Wire Line
	6450 1550 6050 1550
Wire Wire Line
	6450 2050 6050 2050
Wire Wire Line
	6950 1900 6850 1900
Wire Wire Line
	7250 1900 7400 1900
Wire Wire Line
	7400 1900 7400 2050
Connection ~ 7400 2050
Wire Wire Line
	7400 2050 7800 2050
$Comp
L Device:R R3
U 1 1 61AE74E8
P 6600 2550
F 0 "R3" V 6393 2550 50  0000 C CNN
F 1 "2k2" V 6484 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 2550 50  0001 C CNN
F 3 "~" H 6600 2550 50  0001 C CNN
	1    6600 2550
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 61AE7682
P 6600 3050
F 0 "R4" V 6393 3050 50  0000 C CNN
F 1 "2k2" V 6484 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 3050 50  0001 C CNN
F 3 "~" H 6600 3050 50  0001 C CNN
	1    6600 3050
	0    1    1    0   
$EndComp
$Comp
L Device:C C3
U 1 1 61AE768C
P 7100 2400
F 0 "C3" V 6848 2400 50  0000 C CNN
F 1 "1n" V 6939 2400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 2250 50  0001 C CNN
F 3 "~" H 7100 2400 50  0001 C CNN
	1    7100 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 2550 7400 2550
Wire Wire Line
	7250 2400 7400 2400
Wire Wire Line
	7400 2400 7400 2550
Connection ~ 7400 2550
Wire Wire Line
	7400 2550 7800 2550
Wire Wire Line
	6950 2400 6850 2400
Wire Wire Line
	6850 2400 6850 2900
$Comp
L Device:C C4
U 1 1 61AE769D
P 7100 2900
F 0 "C4" V 6848 2900 50  0000 C CNN
F 1 "1n" V 6939 2900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 2750 50  0001 C CNN
F 3 "~" H 7100 2900 50  0001 C CNN
	1    7100 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 3050 7400 3050
Wire Wire Line
	6450 2550 6050 2550
Wire Wire Line
	6450 3050 6050 3050
Wire Wire Line
	6950 2900 6850 2900
Wire Wire Line
	7250 2900 7400 2900
Wire Wire Line
	7400 2900 7400 3050
Connection ~ 7400 3050
Wire Wire Line
	7400 3050 7800 3050
$Comp
L Device:R R5
U 1 1 61AEA1A8
P 6600 3550
F 0 "R5" V 6393 3550 50  0000 C CNN
F 1 "2k2" V 6484 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 3550 50  0001 C CNN
F 3 "~" H 6600 3550 50  0001 C CNN
	1    6600 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 61AEA386
P 6600 4050
F 0 "R6" V 6393 4050 50  0000 C CNN
F 1 "2k2" V 6484 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 4050 50  0001 C CNN
F 3 "~" H 6600 4050 50  0001 C CNN
	1    6600 4050
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 61AEA390
P 7100 3400
F 0 "C5" V 6848 3400 50  0000 C CNN
F 1 "1n" V 6939 3400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 3250 50  0001 C CNN
F 3 "~" H 7100 3400 50  0001 C CNN
	1    7100 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 3550 7400 3550
Wire Wire Line
	7250 3400 7400 3400
Wire Wire Line
	7400 3400 7400 3550
Connection ~ 7400 3550
Wire Wire Line
	7400 3550 7800 3550
Wire Wire Line
	6950 3400 6850 3400
$Comp
L Device:C C6
U 1 1 61AEA3A1
P 7100 3900
F 0 "C6" V 6848 3900 50  0000 C CNN
F 1 "1n" V 6939 3900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 3750 50  0001 C CNN
F 3 "~" H 7100 3900 50  0001 C CNN
	1    7100 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 4050 7400 4050
Wire Wire Line
	6450 3550 6050 3550
Wire Wire Line
	6450 4050 6050 4050
Wire Wire Line
	6950 3900 6850 3900
Wire Wire Line
	7250 3900 7400 3900
Wire Wire Line
	7400 3900 7400 4050
Connection ~ 7400 4050
Wire Wire Line
	7400 4050 7800 4050
$Comp
L Device:R R7
U 1 1 61AEA3B3
P 6600 4550
F 0 "R7" V 6393 4550 50  0000 C CNN
F 1 "2k2" V 6484 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 4550 50  0001 C CNN
F 3 "~" H 6600 4550 50  0001 C CNN
	1    6600 4550
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 61AEA3BD
P 6600 5050
F 0 "R8" V 6393 5050 50  0000 C CNN
F 1 "2k2" V 6484 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 5050 50  0001 C CNN
F 3 "~" H 6600 5050 50  0001 C CNN
	1    6600 5050
	0    1    1    0   
$EndComp
$Comp
L Device:C C7
U 1 1 61AEA3C7
P 7100 4400
F 0 "C7" V 6848 4400 50  0000 C CNN
F 1 "1n" V 6939 4400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 4250 50  0001 C CNN
F 3 "~" H 7100 4400 50  0001 C CNN
	1    7100 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 4550 7400 4550
Wire Wire Line
	7250 4400 7400 4400
Wire Wire Line
	7400 4400 7400 4550
Connection ~ 7400 4550
Wire Wire Line
	7400 4550 7800 4550
Wire Wire Line
	6850 4400 6850 4900
$Comp
L Device:C C8
U 1 1 61AEA3D8
P 7100 4900
F 0 "C8" V 6848 4900 50  0000 C CNN
F 1 "1n" V 6939 4900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7138 4750 50  0001 C CNN
F 3 "~" H 7100 4900 50  0001 C CNN
	1    7100 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 5050 7400 5050
Wire Wire Line
	6450 4550 6050 4550
Wire Wire Line
	6450 5050 6050 5050
Wire Wire Line
	6950 4900 6850 4900
Wire Wire Line
	7250 4900 7400 4900
Wire Wire Line
	7400 4900 7400 5050
Connection ~ 7400 5050
Wire Wire Line
	7400 5050 7800 5050
$Comp
L Device:R R9
U 1 1 61AF4B06
P 9200 1550
F 0 "R9" V 8993 1550 50  0000 C CNN
F 1 "2k2" V 9084 1550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 1550 50  0001 C CNN
F 3 "~" H 9200 1550 50  0001 C CNN
	1    9200 1550
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 61AF4D6C
P 9200 2050
F 0 "R10" V 8993 2050 50  0000 C CNN
F 1 "2k2" V 9084 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 2050 50  0001 C CNN
F 3 "~" H 9200 2050 50  0001 C CNN
	1    9200 2050
	0    1    1    0   
$EndComp
$Comp
L Device:C C9
U 1 1 61AF4D76
P 9700 1400
F 0 "C9" V 9448 1400 50  0000 C CNN
F 1 "1n" V 9539 1400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 1250 50  0001 C CNN
F 3 "~" H 9700 1400 50  0001 C CNN
	1    9700 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 1550 10000 1550
Wire Wire Line
	9850 1400 10000 1400
Wire Wire Line
	10000 1400 10000 1550
Connection ~ 10000 1550
Wire Wire Line
	10000 1550 10400 1550
Wire Wire Line
	9550 1400 9450 1400
Wire Wire Line
	9450 1400 9450 1900
$Comp
L Device:C C10
U 1 1 61AF4D87
P 9700 1900
F 0 "C10" V 9448 1900 50  0000 C CNN
F 1 "1n" V 9539 1900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 1750 50  0001 C CNN
F 3 "~" H 9700 1900 50  0001 C CNN
	1    9700 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 2050 10000 2050
Wire Wire Line
	9050 1550 8650 1550
Wire Wire Line
	9050 2050 8650 2050
Wire Wire Line
	9550 1900 9450 1900
Wire Wire Line
	9850 1900 10000 1900
Wire Wire Line
	10000 1900 10000 2050
Connection ~ 10000 2050
Wire Wire Line
	10000 2050 10400 2050
$Comp
L Device:R R11
U 1 1 61AF4D99
P 9200 2550
F 0 "R11" V 8993 2550 50  0000 C CNN
F 1 "2k2" V 9084 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 2550 50  0001 C CNN
F 3 "~" H 9200 2550 50  0001 C CNN
	1    9200 2550
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 61AF4DA3
P 9200 3050
F 0 "R12" V 8993 3050 50  0000 C CNN
F 1 "2k2" V 9084 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 3050 50  0001 C CNN
F 3 "~" H 9200 3050 50  0001 C CNN
	1    9200 3050
	0    1    1    0   
$EndComp
$Comp
L Device:C C11
U 1 1 61AF4DAD
P 9700 2400
F 0 "C11" V 9448 2400 50  0000 C CNN
F 1 "1n" V 9539 2400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 2250 50  0001 C CNN
F 3 "~" H 9700 2400 50  0001 C CNN
	1    9700 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 2550 10000 2550
Wire Wire Line
	9850 2400 10000 2400
Wire Wire Line
	10000 2400 10000 2550
Connection ~ 10000 2550
Wire Wire Line
	10000 2550 10400 2550
Wire Wire Line
	9550 2400 9450 2400
Wire Wire Line
	9450 2400 9450 2900
$Comp
L Device:C C12
U 1 1 61AF4DBE
P 9700 2900
F 0 "C12" V 9448 2900 50  0000 C CNN
F 1 "1n" V 9539 2900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 2750 50  0001 C CNN
F 3 "~" H 9700 2900 50  0001 C CNN
	1    9700 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 3050 10000 3050
Wire Wire Line
	9050 2550 8650 2550
Wire Wire Line
	9050 3050 8650 3050
Wire Wire Line
	9550 2900 9450 2900
Wire Wire Line
	9850 2900 10000 2900
Wire Wire Line
	10000 2900 10000 3050
Connection ~ 10000 3050
Wire Wire Line
	10000 3050 10400 3050
$Comp
L Device:R R13
U 1 1 61AF4DD0
P 9200 3550
F 0 "R13" V 8993 3550 50  0000 C CNN
F 1 "2k2" V 9084 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 3550 50  0001 C CNN
F 3 "~" H 9200 3550 50  0001 C CNN
	1    9200 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 61AF4DDA
P 9200 4050
F 0 "R14" V 8993 4050 50  0000 C CNN
F 1 "2k2" V 9084 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 4050 50  0001 C CNN
F 3 "~" H 9200 4050 50  0001 C CNN
	1    9200 4050
	0    1    1    0   
$EndComp
$Comp
L Device:C C13
U 1 1 61AF4DE4
P 9700 3400
F 0 "C13" V 9448 3400 50  0000 C CNN
F 1 "1n" V 9539 3400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 3250 50  0001 C CNN
F 3 "~" H 9700 3400 50  0001 C CNN
	1    9700 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 3550 10000 3550
Wire Wire Line
	9850 3400 10000 3400
Wire Wire Line
	10000 3400 10000 3550
Connection ~ 10000 3550
Wire Wire Line
	10000 3550 10400 3550
Wire Wire Line
	9550 3400 9450 3400
Wire Wire Line
	9450 3400 9450 3900
$Comp
L Device:C C14
U 1 1 61AF4DF5
P 9700 3900
F 0 "C14" V 9448 3900 50  0000 C CNN
F 1 "1n" V 9539 3900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 3750 50  0001 C CNN
F 3 "~" H 9700 3900 50  0001 C CNN
	1    9700 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 4050 10000 4050
Wire Wire Line
	9050 3550 8650 3550
Wire Wire Line
	9050 4050 8650 4050
Wire Wire Line
	9550 3900 9450 3900
Wire Wire Line
	9850 3900 10000 3900
Wire Wire Line
	10000 3900 10000 4050
Connection ~ 10000 4050
Wire Wire Line
	10000 4050 10400 4050
$Comp
L Device:R R15
U 1 1 61AF4E07
P 9200 4550
F 0 "R15" V 8993 4550 50  0000 C CNN
F 1 "2k2" V 9084 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 4550 50  0001 C CNN
F 3 "~" H 9200 4550 50  0001 C CNN
	1    9200 4550
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 61AF4E11
P 9200 5050
F 0 "R16" V 8993 5050 50  0000 C CNN
F 1 "2k2" V 9084 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9130 5050 50  0001 C CNN
F 3 "~" H 9200 5050 50  0001 C CNN
	1    9200 5050
	0    1    1    0   
$EndComp
$Comp
L Device:C C15
U 1 1 61AF4E1B
P 9700 4400
F 0 "C15" V 9448 4400 50  0000 C CNN
F 1 "1n" V 9539 4400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 4250 50  0001 C CNN
F 3 "~" H 9700 4400 50  0001 C CNN
	1    9700 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 4550 10000 4550
Wire Wire Line
	9850 4400 10000 4400
Wire Wire Line
	10000 4400 10000 4550
Connection ~ 10000 4550
Wire Wire Line
	10000 4550 10400 4550
Wire Wire Line
	9550 4400 9450 4400
Wire Wire Line
	9450 4400 9450 4900
$Comp
L Device:C C16
U 1 1 61AF4E2C
P 9700 4900
F 0 "C16" V 9448 4900 50  0000 C CNN
F 1 "1n" V 9539 4900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9738 4750 50  0001 C CNN
F 3 "~" H 9700 4900 50  0001 C CNN
	1    9700 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	9350 5050 10000 5050
Wire Wire Line
	9050 4550 8650 4550
Wire Wire Line
	9050 5050 8650 5050
Wire Wire Line
	9550 4900 9450 4900
Wire Wire Line
	9850 4900 10000 4900
Wire Wire Line
	10000 4900 10000 5050
Connection ~ 10000 5050
Wire Wire Line
	10000 5050 10400 5050
Wire Wire Line
	6850 1900 6850 2400
Connection ~ 6850 1900
Connection ~ 6850 2400
Wire Wire Line
	6850 2900 6850 3400
Connection ~ 6850 2900
Connection ~ 6850 3400
Wire Wire Line
	6950 4400 6850 4400
Wire Wire Line
	6850 3400 6850 3900
Wire Wire Line
	6850 3900 6850 4400
Connection ~ 6850 3900
Connection ~ 6850 4400
Wire Wire Line
	6850 4900 6850 5400
Connection ~ 6850 4900
Wire Wire Line
	9450 1900 9450 2400
Connection ~ 9450 1900
Connection ~ 9450 2400
Wire Wire Line
	9450 2900 9450 3400
Connection ~ 9450 2900
Connection ~ 9450 3400
Wire Wire Line
	9450 3900 9450 4400
Connection ~ 9450 3900
Connection ~ 9450 4400
Wire Wire Line
	9450 4900 9450 5400
Connection ~ 9450 4900
$Comp
L power:GND #PWR08
U 1 1 61B35696
P 6850 5400
F 0 "#PWR08" H 6850 5150 50  0001 C CNN
F 1 "GND" H 6855 5227 50  0000 C CNN
F 2 "" H 6850 5400 50  0001 C CNN
F 3 "" H 6850 5400 50  0001 C CNN
	1    6850 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 61B3647E
P 9450 5400
F 0 "#PWR09" H 9450 5150 50  0001 C CNN
F 1 "GND" H 9455 5227 50  0000 C CNN
F 2 "" H 9450 5400 50  0001 C CNN
F 3 "" H 9450 5400 50  0001 C CNN
	1    9450 5400
	1    0    0    -1  
$EndComp
$Comp
L OpenBCI_ESP32:TPD4E1B06 U2
U 1 1 61B56178
P 2200 4150
F 0 "U2" V 2488 4246 50  0000 L CNN
F 1 "TPD4E1B06" V 1850 4000 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 2500 4150 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 2325 4475 50  0001 C CNN
	1    2200 4150
	0    -1   1    0   
$EndComp
$Comp
L OpenBCI_ESP32:TPD4E1B06 U3
U 1 1 61B5D454
P 2200 5000
F 0 "U3" V 2488 5096 50  0000 L CNN
F 1 "TPD4E1B06" V 1850 4850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 2500 5000 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 2325 5325 50  0001 C CNN
	1    2200 5000
	0    -1   1    0   
$EndComp
$Comp
L OpenBCI_ESP32:TPD4E1B06 U4
U 1 1 61B5ED65
P 3700 3350
F 0 "U4" V 3988 3446 50  0000 L CNN
F 1 "TPD4E1B06" V 3350 3200 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 4000 3350 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 3825 3675 50  0001 C CNN
	1    3700 3350
	0    -1   1    0   
$EndComp
$Comp
L OpenBCI_ESP32:TPD4E1B06 U5
U 1 1 61B5F16F
P 3700 4150
F 0 "U5" V 3988 4246 50  0000 L CNN
F 1 "TPD4E1B06" V 3350 4000 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 4000 4150 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 3825 4475 50  0001 C CNN
	1    3700 4150
	0    -1   1    0   
$EndComp
$Comp
L OpenBCI_ESP32:TPD4E1B06 U6
U 1 1 61B5F179
P 3700 5000
F 0 "U6" V 3988 5096 50  0000 L CNN
F 1 "TPD4E1B06" V 3350 4850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 4000 5000 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd4e02b04.pdf" H 3825 5325 50  0001 C CNN
	1    3700 5000
	0    -1   1    0   
$EndComp
Wire Wire Line
	2400 3350 2600 3350
Wire Wire Line
	2600 3350 2600 4150
Wire Wire Line
	3900 3350 4100 3350
Wire Wire Line
	4100 3350 4100 4150
$Comp
L power:GND #PWR05
U 1 1 61B7555E
P 4100 5500
F 0 "#PWR05" H 4100 5250 50  0001 C CNN
F 1 "GND" H 4105 5327 50  0000 C CNN
F 2 "" H 4100 5500 50  0001 C CNN
F 3 "" H 4100 5500 50  0001 C CNN
	1    4100 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 61B75DD2
P 2600 5500
F 0 "#PWR03" H 2600 5250 50  0001 C CNN
F 1 "GND" H 2605 5327 50  0000 C CNN
F 2 "" H 2600 5500 50  0001 C CNN
F 3 "" H 2600 5500 50  0001 C CNN
	1    2600 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3150 2950 3150
Wire Wire Line
	3400 3250 2950 3250
Wire Wire Line
	3400 3450 2950 3450
Wire Wire Line
	3400 3550 2950 3550
Wire Wire Line
	3400 3950 2950 3950
Wire Wire Line
	3400 4050 2950 4050
Wire Wire Line
	3400 4250 2950 4250
Wire Wire Line
	3400 4350 2950 4350
Wire Wire Line
	3400 4800 2950 4800
Wire Wire Line
	3400 4900 2950 4900
Wire Wire Line
	3400 5100 2950 5100
Wire Wire Line
	3400 5200 2950 5200
Wire Wire Line
	1900 3950 1350 3950
Wire Wire Line
	1900 4050 1350 4050
Wire Wire Line
	1900 4250 1350 4250
Wire Wire Line
	1900 4350 1350 4350
Wire Wire Line
	1900 4800 1350 4800
Wire Wire Line
	1900 4900 1350 4900
Wire Wire Line
	1900 5100 1350 5100
Wire Wire Line
	1900 5200 1350 5200
Text Label 6050 1550 0    50   ~ 0
1P
Text Label 8650 1550 0    50   ~ 0
1N
Text Label 6050 2050 0    50   ~ 0
2P
Text Label 6050 2550 0    50   ~ 0
3P
Text Label 6050 3050 0    50   ~ 0
4P
Text Label 6050 3550 0    50   ~ 0
5P
Text Label 6050 4050 0    50   ~ 0
6P
Text Label 6050 4550 0    50   ~ 0
7P
Text Label 6050 5050 0    50   ~ 0
8P
Text Label 8650 2050 0    50   ~ 0
2N
Text Label 8650 2550 0    50   ~ 0
3N
Text Label 8650 3050 0    50   ~ 0
4N
Text Label 8650 3550 0    50   ~ 0
5N
Text Label 8650 4050 0    50   ~ 0
6N
Text Label 8650 4550 0    50   ~ 0
7N
Text Label 8650 5050 0    50   ~ 0
8N
Wire Wire Line
	2400 4150 2600 4150
Connection ~ 2600 4150
Wire Wire Line
	2600 4150 2600 5000
Wire Wire Line
	2400 5000 2600 5000
Connection ~ 2600 5000
Wire Wire Line
	2600 5000 2600 5500
Wire Wire Line
	3900 4150 4100 4150
Connection ~ 4100 4150
Wire Wire Line
	4100 4150 4100 5000
Wire Wire Line
	3900 5000 4100 5000
Connection ~ 4100 5000
Wire Wire Line
	4100 5000 4100 5500
Wire Wire Line
	1900 3450 1350 3450
NoConn ~ 1900 3250
NoConn ~ 1900 3550
Wire Wire Line
	1700 3050 1700 3150
Wire Wire Line
	1700 3150 1900 3150
Text Label 1350 3450 0    50   ~ 0
BIAS
Text Label 1350 3950 0    50   ~ 0
1P
Text Label 1350 4350 0    50   ~ 0
2P
$Comp
L power:-2V5 #PWR0101
U 1 1 61A0CA40
P 1700 3050
F 0 "#PWR0101" H 1700 3150 50  0001 C CNN
F 1 "-2V5" H 1715 3223 50  0000 C CNN
F 2 "" H 1700 3050 50  0001 C CNN
F 3 "" H 1700 3050 50  0001 C CNN
	1    1700 3050
	1    0    0    -1  
$EndComp
Text Label 1350 4250 0    50   ~ 0
2N
Text Label 1350 4050 0    50   ~ 0
1N
Text HLabel 7800 1550 2    50   Input ~ 0
IN1P
Text HLabel 10400 1550 2    50   Input ~ 0
IN1N
Text HLabel 7800 2050 2    50   Input ~ 0
IN2P
Text HLabel 7800 2550 2    50   Input ~ 0
IN3P
Text HLabel 10400 2050 2    50   Input ~ 0
IN2N
Text HLabel 10400 2550 2    50   Input ~ 0
IN3N
Text HLabel 10400 3050 2    50   Input ~ 0
IN4N
Text HLabel 10400 3550 2    50   Input ~ 0
IN5N
Text HLabel 10400 4050 2    50   Input ~ 0
IN6N
Text HLabel 10400 4550 2    50   Input ~ 0
IN7N
Text HLabel 10400 5050 2    50   Input ~ 0
IN8N
Text HLabel 7800 3050 2    50   Input ~ 0
IN4P
Text HLabel 7800 3550 2    50   Input ~ 0
IN5P
Text HLabel 7800 4050 2    50   Input ~ 0
IN6P
Text HLabel 7800 4550 2    50   Input ~ 0
IN7P
Text HLabel 7800 5050 2    50   Input ~ 0
IN8P
Text Label 2950 3950 0    50   ~ 0
8N
Text Label 2950 4050 0    50   ~ 0
8P
Text Label 2950 4250 0    50   ~ 0
7P
Text Label 2950 4350 0    50   ~ 0
7N
$EndSCHEMATC

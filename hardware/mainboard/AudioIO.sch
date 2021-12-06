EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
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
L Amplifier_Operational:TL074 U?
U 3 1 61C4719A
P 5650 1400
AR Path="/619B044B/61C4719A" Ref="U?"  Part="1" 
AR Path="/61C4719A" Ref="U?"  Part="1" 
AR Path="/61C2C7F7/61C4719A" Ref="U4"  Part="3" 
F 0 "U4" H 5650 1550 50  0000 L CNN
F 1 "TL074" H 5650 1250 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5600 1500 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 5700 1600 50  0001 C CNN
	3    5650 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+12VA #PWR?
U 1 1 61C471A0
P 7850 3150
AR Path="/619B044B/61C471A0" Ref="#PWR?"  Part="1" 
AR Path="/61C471A0" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C471A0" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 7850 3000 50  0001 C CNN
F 1 "+12VA" H 7865 3323 50  0000 C CNN
F 2 "" H 7850 3150 50  0001 C CNN
F 3 "" H 7850 3150 50  0001 C CNN
	1    7850 3150
	1    0    0    -1  
$EndComp
$Comp
L power:-12VA #PWR?
U 1 1 61C471A6
P 7850 3750
AR Path="/619B044B/61C471A6" Ref="#PWR?"  Part="1" 
AR Path="/61C471A6" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C471A6" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 7850 3600 50  0001 C CNN
F 1 "-12VA" H 7865 3923 50  0000 C CNN
F 2 "" H 7850 3750 50  0001 C CNN
F 3 "" H 7850 3750 50  0001 C CNN
	1    7850 3750
	-1   0    0    1   
$EndComp
Wire Wire Line
	4850 2950 4850 1500
Connection ~ 4850 1500
$Comp
L Device:R R?
U 1 1 61C471AE
P 5050 1500
AR Path="/619B044B/61C471AE" Ref="R?"  Part="1" 
AR Path="/61C471AE" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471AE" Ref="R10"  Part="1" 
F 0 "R10" V 5150 1500 50  0000 C CNN
F 1 "100k" V 5250 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4980 1500 50  0001 C CNN
F 3 "" H 5050 1500 50  0001 C CNN
	1    5050 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 1500 4900 1500
Wire Wire Line
	5200 1500 5250 1500
Wire Wire Line
	5250 1500 5250 1700
Wire Wire Line
	5250 1700 5600 1700
Connection ~ 5250 1500
Wire Wire Line
	5250 1500 5350 1500
$Comp
L Device:R R?
U 1 1 61C471BA
P 5750 1700
AR Path="/619B044B/61C471BA" Ref="R?"  Part="1" 
AR Path="/61C471BA" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471BA" Ref="R14"  Part="1" 
F 0 "R14" V 5850 1700 50  0000 C CNN
F 1 "10k" V 5950 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5680 1700 50  0001 C CNN
F 3 "" H 5750 1700 50  0001 C CNN
	1    5750 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	5900 1700 6050 1700
Wire Wire Line
	6050 1700 6050 1400
Wire Wire Line
	6050 1400 5950 1400
$Comp
L power:GNDA #PWR?
U 1 1 61C471C3
P 5250 1300
AR Path="/619B044B/61C471C3" Ref="#PWR?"  Part="1" 
AR Path="/61C471C3" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C471C3" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 5250 1050 50  0001 C CNN
F 1 "GNDA" H 5100 1300 50  0000 C CNN
F 2 "" H 5250 1300 50  0001 C CNN
F 3 "" H 5250 1300 50  0001 C CNN
	1    5250 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1300 5350 1300
Connection ~ 6050 1400
Text GLabel 6250 1400 2    50   Output ~ 0
AUDIO_ADC_L
$Comp
L Amplifier_Operational:TL074 U?
U 4 1 61C471CC
P 5650 2750
AR Path="/619B044B/61C471CC" Ref="U?"  Part="1" 
AR Path="/61C471CC" Ref="U?"  Part="1" 
AR Path="/61C2C7F7/61C471CC" Ref="U4"  Part="4" 
F 0 "U4" H 5650 2900 50  0000 L CNN
F 1 "TL074" H 5650 2600 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5600 2850 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 5700 2950 50  0001 C CNN
	4    5650 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C471D2
P 5050 2850
AR Path="/619B044B/61C471D2" Ref="R?"  Part="1" 
AR Path="/61C471D2" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471D2" Ref="R11"  Part="1" 
F 0 "R11" V 5150 2850 50  0000 C CNN
F 1 "100k" V 5250 2850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4980 2850 50  0001 C CNN
F 3 "" H 5050 2850 50  0001 C CNN
	1    5050 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	5200 2850 5250 2850
Wire Wire Line
	5250 2850 5250 3050
Wire Wire Line
	5250 3050 5600 3050
Connection ~ 5250 2850
Wire Wire Line
	5250 2850 5350 2850
$Comp
L Device:R R?
U 1 1 61C471DD
P 5750 3050
AR Path="/619B044B/61C471DD" Ref="R?"  Part="1" 
AR Path="/61C471DD" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471DD" Ref="R15"  Part="1" 
F 0 "R15" V 5850 3050 50  0000 C CNN
F 1 "10k" V 5950 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5680 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	5900 3050 6050 3050
Wire Wire Line
	6050 3050 6050 2750
Wire Wire Line
	6050 2750 5950 2750
$Comp
L power:GNDA #PWR?
U 1 1 61C471E6
P 5250 2650
AR Path="/619B044B/61C471E6" Ref="#PWR?"  Part="1" 
AR Path="/61C471E6" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C471E6" Ref="#PWR023"  Part="1" 
F 0 "#PWR023" H 5250 2400 50  0001 C CNN
F 1 "GNDA" H 5100 2650 50  0000 C CNN
F 2 "" H 5250 2650 50  0001 C CNN
F 3 "" H 5250 2650 50  0001 C CNN
	1    5250 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2650 5350 2650
Connection ~ 6050 2750
$Comp
L Amplifier_Operational:TL074 U?
U 2 1 61C471EE
P 4700 3950
AR Path="/619B044B/61C471EE" Ref="U?"  Part="1" 
AR Path="/61C471EE" Ref="U?"  Part="1" 
AR Path="/61C2C7F7/61C471EE" Ref="U4"  Part="2" 
F 0 "U4" H 4700 4100 50  0000 L CNN
F 1 "TL074" H 4700 3800 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 4650 4050 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 4750 4150 50  0001 C CNN
	2    4700 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C471F4
P 4050 4050
AR Path="/619B044B/61C471F4" Ref="R?"  Part="1" 
AR Path="/61C471F4" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471F4" Ref="R6"  Part="1" 
F 0 "R6" V 4150 4050 50  0000 C CNN
F 1 "10k" V 4250 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3980 4050 50  0001 C CNN
F 3 "" H 4050 4050 50  0001 C CNN
	1    4050 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	4200 4050 4300 4050
Wire Wire Line
	4300 4050 4300 4250
Wire Wire Line
	4300 4250 4650 4250
Connection ~ 4300 4050
Wire Wire Line
	4300 4050 4400 4050
$Comp
L Device:R R?
U 1 1 61C471FF
P 4800 4250
AR Path="/619B044B/61C471FF" Ref="R?"  Part="1" 
AR Path="/61C471FF" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C471FF" Ref="R8"  Part="1" 
F 0 "R8" V 4900 4250 50  0000 C CNN
F 1 "100k" V 5000 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4730 4250 50  0001 C CNN
F 3 "" H 4800 4250 50  0001 C CNN
	1    4800 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 4250 5100 4250
Wire Wire Line
	5100 4250 5100 3950
Wire Wire Line
	5100 3950 5000 3950
$Comp
L power:GNDA #PWR?
U 1 1 61C47208
P 4300 3850
AR Path="/619B044B/61C47208" Ref="#PWR?"  Part="1" 
AR Path="/61C47208" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47208" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 4300 3600 50  0001 C CNN
F 1 "GNDA" H 4150 3850 50  0000 C CNN
F 2 "" H 4300 3850 50  0001 C CNN
F 3 "" H 4300 3850 50  0001 C CNN
	1    4300 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3850 4400 3850
Wire Wire Line
	5100 3950 5200 3950
Connection ~ 5100 3950
$Comp
L Amplifier_Operational:TL074 U?
U 1 1 61C47211
P 4700 5250
AR Path="/619B044B/61C47211" Ref="U?"  Part="1" 
AR Path="/61C47211" Ref="U?"  Part="1" 
AR Path="/61C2C7F7/61C47211" Ref="U4"  Part="1" 
F 0 "U4" H 4700 5400 50  0000 L CNN
F 1 "TL074" H 4700 5100 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 4650 5350 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 4750 5450 50  0001 C CNN
	1    4700 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C47217
P 4050 5350
AR Path="/619B044B/61C47217" Ref="R?"  Part="1" 
AR Path="/61C47217" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C47217" Ref="R7"  Part="1" 
F 0 "R7" V 4150 5350 50  0000 C CNN
F 1 "10k" V 4250 5350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3980 5350 50  0001 C CNN
F 3 "" H 4050 5350 50  0001 C CNN
	1    4050 5350
	0    1    1    0   
$EndComp
Wire Wire Line
	4200 5350 4300 5350
Wire Wire Line
	4300 5350 4300 5550
Wire Wire Line
	4300 5550 4650 5550
Connection ~ 4300 5350
Wire Wire Line
	4300 5350 4400 5350
$Comp
L Device:R R?
U 1 1 61C47222
P 4800 5550
AR Path="/619B044B/61C47222" Ref="R?"  Part="1" 
AR Path="/61C47222" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C47222" Ref="R9"  Part="1" 
F 0 "R9" V 4900 5550 50  0000 C CNN
F 1 "100k" V 5000 5550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4730 5550 50  0001 C CNN
F 3 "" H 4800 5550 50  0001 C CNN
	1    4800 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 5550 5100 5550
Wire Wire Line
	5100 5550 5100 5250
Wire Wire Line
	5100 5250 5000 5250
$Comp
L power:GNDA #PWR?
U 1 1 61C4722B
P 4300 5150
AR Path="/619B044B/61C4722B" Ref="#PWR?"  Part="1" 
AR Path="/61C4722B" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C4722B" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 4300 4900 50  0001 C CNN
F 1 "GNDA" H 4150 5150 50  0000 C CNN
F 2 "" H 4300 5150 50  0001 C CNN
F 3 "" H 4300 5150 50  0001 C CNN
	1    4300 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 5150 4400 5150
Wire Wire Line
	5100 5250 5200 5250
Connection ~ 5100 5250
$Comp
L Device:R R?
U 1 1 61C47234
P 5350 3950
AR Path="/619B044B/61C47234" Ref="R?"  Part="1" 
AR Path="/61C47234" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C47234" Ref="R12"  Part="1" 
F 0 "R12" V 5450 3950 50  0000 C CNN
F 1 "1k" V 5550 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5280 3950 50  0001 C CNN
F 3 "" H 5350 3950 50  0001 C CNN
	1    5350 3950
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61C4723A
P 5350 5250
AR Path="/619B044B/61C4723A" Ref="R?"  Part="1" 
AR Path="/61C4723A" Ref="R?"  Part="1" 
AR Path="/61C2C7F7/61C4723A" Ref="R13"  Part="1" 
F 0 "R13" V 5450 5250 50  0000 C CNN
F 1 "1k" V 5550 5250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5280 5250 50  0001 C CNN
F 3 "" H 5350 5250 50  0001 C CNN
	1    5350 5250
	0    1    1    0   
$EndComp
Text GLabel 3800 4050 0    50   Input ~ 0
AUDIO_DAC_L
Wire Wire Line
	3800 4050 3900 4050
Text GLabel 3800 5350 0    50   Input ~ 0
AUDIO_DAC_R
Wire Wire Line
	3800 5350 3900 5350
Text GLabel 6250 2750 2    50   Output ~ 0
AUDIO_ADC_R
Wire Wire Line
	6050 1400 6250 1400
Wire Wire Line
	6050 2750 6250 2750
$Comp
L power:GNDA #PWR?
U 1 1 61C47247
P 4550 1800
AR Path="/619B044B/61C47247" Ref="#PWR?"  Part="1" 
AR Path="/61C47247" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47247" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 4550 1550 50  0001 C CNN
F 1 "GNDA" H 4700 1800 50  0000 C CNN
F 2 "" H 4550 1800 50  0001 C CNN
F 3 "" H 4550 1800 50  0001 C CNN
	1    4550 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 1500 4500 1500
Wire Wire Line
	4500 1750 4550 1750
Wire Wire Line
	4550 1750 4550 1800
$Comp
L ell_audio_conn:JACK_TS_3PINS J?
U 1 1 61C47250
P 4100 1550
AR Path="/61C47250" Ref="J?"  Part="1" 
AR Path="/5EDCCDBA/61C47250" Ref="J?"  Part="1" 
AR Path="/619B044B/61C47250" Ref="J?"  Part="1" 
AR Path="/61C2C7F7/61C47250" Ref="J3"  Part="1" 
F 0 "J3" H 3622 1584 50  0000 R CNN
F 1 "PJ398SM" H 3622 1675 50  0000 R CNN
F 2 "ell_connectors:THONKICONN" H 4083 1883 50  0001 C CNN
F 3 "" H 4200 1400 50  0000 C CNN
	1    4100 1550
	1    0    0    1   
$EndComp
NoConn ~ 4500 1600
$Comp
L power:GNDA #PWR?
U 1 1 61C47257
P 4550 3150
AR Path="/619B044B/61C47257" Ref="#PWR?"  Part="1" 
AR Path="/61C47257" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47257" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 4550 2900 50  0001 C CNN
F 1 "GNDA" H 4700 3150 50  0000 C CNN
F 2 "" H 4550 3150 50  0001 C CNN
F 3 "" H 4550 3150 50  0001 C CNN
	1    4550 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3100 4550 3100
Wire Wire Line
	4550 3100 4550 3150
Wire Wire Line
	4500 2850 4900 2850
$Comp
L ell_audio_conn:JACK_TS_3PINS J?
U 1 1 61C47260
P 4100 2900
AR Path="/61C47260" Ref="J?"  Part="1" 
AR Path="/5EDCCDBA/61C47260" Ref="J?"  Part="1" 
AR Path="/619B044B/61C47260" Ref="J?"  Part="1" 
AR Path="/61C2C7F7/61C47260" Ref="J4"  Part="1" 
F 0 "J4" H 3622 2934 50  0000 R CNN
F 1 "PJ398SM" H 3622 3025 50  0000 R CNN
F 2 "ell_connectors:THONKICONN" H 4083 3233 50  0001 C CNN
F 3 "" H 4200 2750 50  0000 C CNN
	1    4100 2900
	1    0    0    1   
$EndComp
Wire Wire Line
	4500 2950 4850 2950
$Comp
L power:GNDA #PWR?
U 1 1 61C47267
P 5550 4250
AR Path="/619B044B/61C47267" Ref="#PWR?"  Part="1" 
AR Path="/61C47267" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47267" Ref="#PWR024"  Part="1" 
F 0 "#PWR024" H 5550 4000 50  0001 C CNN
F 1 "GNDA" H 5700 4250 50  0000 C CNN
F 2 "" H 5550 4250 50  0001 C CNN
F 3 "" H 5550 4250 50  0001 C CNN
	1    5550 4250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 3950 5600 3950
Wire Wire Line
	5600 4200 5550 4200
Wire Wire Line
	5550 4200 5550 4250
$Comp
L ell_audio_conn:JACK_TS_3PINS J?
U 1 1 61C47270
P 6000 4000
AR Path="/61C47270" Ref="J?"  Part="1" 
AR Path="/5EDCCDBA/61C47270" Ref="J?"  Part="1" 
AR Path="/619B044B/61C47270" Ref="J?"  Part="1" 
AR Path="/61C2C7F7/61C47270" Ref="J5"  Part="1" 
F 0 "J5" H 5522 4034 50  0000 R CNN
F 1 "PJ398SM" H 5522 4125 50  0000 R CNN
F 2 "ell_connectors:THONKICONN" H 5983 4333 50  0001 C CNN
F 3 "" H 6100 3850 50  0000 C CNN
	1    6000 4000
	-1   0    0    1   
$EndComp
NoConn ~ 5600 4050
$Comp
L power:GNDA #PWR?
U 1 1 61C47277
P 5550 5550
AR Path="/619B044B/61C47277" Ref="#PWR?"  Part="1" 
AR Path="/61C47277" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47277" Ref="#PWR025"  Part="1" 
F 0 "#PWR025" H 5550 5300 50  0001 C CNN
F 1 "GNDA" H 5700 5550 50  0000 C CNN
F 2 "" H 5550 5550 50  0001 C CNN
F 3 "" H 5550 5550 50  0001 C CNN
	1    5550 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 5250 5600 5250
Wire Wire Line
	5600 5500 5550 5500
Wire Wire Line
	5550 5500 5550 5550
$Comp
L ell_audio_conn:JACK_TS_3PINS J?
U 1 1 61C47280
P 6000 5300
AR Path="/61C47280" Ref="J?"  Part="1" 
AR Path="/5EDCCDBA/61C47280" Ref="J?"  Part="1" 
AR Path="/619B044B/61C47280" Ref="J?"  Part="1" 
AR Path="/61C2C7F7/61C47280" Ref="J6"  Part="1" 
F 0 "J6" H 5522 5334 50  0000 R CNN
F 1 "PJ398SM" H 5522 5425 50  0000 R CNN
F 2 "ell_connectors:THONKICONN" H 5983 5633 50  0001 C CNN
F 3 "" H 6100 5150 50  0000 C CNN
	1    6000 5300
	-1   0    0    1   
$EndComp
NoConn ~ 5600 5350
$Comp
L Device:C C?
U 1 1 61C47287
P 7400 3300
AR Path="/619B044B/61C47287" Ref="C?"  Part="1" 
AR Path="/61C47287" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/61C47287" Ref="C11"  Part="1" 
F 0 "C11" H 7515 3346 50  0000 L CNN
F 1 "100n" H 7515 3255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 7438 3150 50  0001 C CNN
F 3 "" H 7400 3300 50  0001 C CNN
	1    7400 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+12VA #PWR?
U 1 1 61C4728D
P 7400 3150
AR Path="/619B044B/61C4728D" Ref="#PWR?"  Part="1" 
AR Path="/61C4728D" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C4728D" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 7400 3000 50  0001 C CNN
F 1 "+12VA" H 7415 3323 50  0000 C CNN
F 2 "" H 7400 3150 50  0001 C CNN
F 3 "" H 7400 3150 50  0001 C CNN
	1    7400 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 61C47293
P 7200 3550
AR Path="/619B044B/61C47293" Ref="#PWR?"  Part="1" 
AR Path="/61C47293" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C47293" Ref="#PWR026"  Part="1" 
F 0 "#PWR026" H 7200 3300 50  0001 C CNN
F 1 "GNDA" H 7205 3377 50  0000 C CNN
F 2 "" H 7200 3550 50  0001 C CNN
F 3 "" H 7200 3550 50  0001 C CNN
	1    7200 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61C47299
P 7400 3700
AR Path="/619B044B/61C47299" Ref="C?"  Part="1" 
AR Path="/61C47299" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/61C47299" Ref="C12"  Part="1" 
F 0 "C12" H 7515 3746 50  0000 L CNN
F 1 "100n" H 7515 3655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 7438 3550 50  0001 C CNN
F 3 "" H 7400 3700 50  0001 C CNN
	1    7400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3550 7200 3500
Wire Wire Line
	7200 3500 7400 3500
Wire Wire Line
	7400 3500 7400 3450
Wire Wire Line
	7400 3500 7400 3550
Connection ~ 7400 3500
$Comp
L power:-12VA #PWR?
U 1 1 61C472A4
P 7400 3850
AR Path="/619B044B/61C472A4" Ref="#PWR?"  Part="1" 
AR Path="/61C472A4" Ref="#PWR?"  Part="1" 
AR Path="/61C2C7F7/61C472A4" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 7400 3700 50  0001 C CNN
F 1 "-12VA" H 7415 4023 50  0000 C CNN
F 2 "" H 7400 3850 50  0001 C CNN
F 3 "" H 7400 3850 50  0001 C CNN
	1    7400 3850
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 5 1 61C472AA
P 7950 3450
AR Path="/619B044B/61C472AA" Ref="U?"  Part="5" 
AR Path="/61C472AA" Ref="U?"  Part="5" 
AR Path="/61C2C7F7/61C472AA" Ref="U4"  Part="5" 
F 0 "U4" H 7908 3496 50  0000 L CNN
F 1 "TL074" H 7908 3405 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7900 3550 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8000 3650 50  0001 C CNN
	5    7950 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6224FBCF
P 5750 2050
AR Path="/619B044B/6224FBCF" Ref="C?"  Part="1" 
AR Path="/6224FBCF" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/6224FBCF" Ref="C59"  Part="1" 
F 0 "C59" H 5865 2096 50  0000 L CNN
F 1 "560p" H 5865 2005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5788 1900 50  0001 C CNN
F 3 "" H 5750 2050 50  0001 C CNN
	1    5750 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	5600 2050 5250 2050
Wire Wire Line
	5250 2050 5250 1700
Connection ~ 5250 1700
Wire Wire Line
	5900 2050 6050 2050
Wire Wire Line
	6050 2050 6050 1700
Connection ~ 6050 1700
$Comp
L Device:C C?
U 1 1 6225A6F4
P 5750 3400
AR Path="/619B044B/6225A6F4" Ref="C?"  Part="1" 
AR Path="/6225A6F4" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/6225A6F4" Ref="C60"  Part="1" 
F 0 "C60" H 5865 3446 50  0000 L CNN
F 1 "560p" H 5865 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5788 3250 50  0001 C CNN
F 3 "" H 5750 3400 50  0001 C CNN
	1    5750 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	5600 3400 5250 3400
Wire Wire Line
	5250 3400 5250 3050
Wire Wire Line
	5900 3400 6050 3400
Wire Wire Line
	6050 3400 6050 3050
Connection ~ 6050 3050
Connection ~ 5250 3050
$Comp
L Device:C C?
U 1 1 62266216
P 4800 4600
AR Path="/619B044B/62266216" Ref="C?"  Part="1" 
AR Path="/62266216" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/62266216" Ref="C57"  Part="1" 
F 0 "C57" H 4915 4646 50  0000 L CNN
F 1 "560p" H 4915 4555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 4838 4450 50  0001 C CNN
F 3 "" H 4800 4600 50  0001 C CNN
	1    4800 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 4600 4300 4600
Wire Wire Line
	4300 4600 4300 4250
Wire Wire Line
	4950 4600 5100 4600
Wire Wire Line
	5100 4600 5100 4250
$Comp
L Device:C C?
U 1 1 62267FBC
P 4800 5900
AR Path="/619B044B/62267FBC" Ref="C?"  Part="1" 
AR Path="/62267FBC" Ref="C?"  Part="1" 
AR Path="/61C2C7F7/62267FBC" Ref="C58"  Part="1" 
F 0 "C58" H 4915 5946 50  0000 L CNN
F 1 "560p" H 4915 5855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 4838 5750 50  0001 C CNN
F 3 "" H 4800 5900 50  0001 C CNN
	1    4800 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 5900 4300 5900
Wire Wire Line
	4300 5900 4300 5550
Wire Wire Line
	4950 5900 5100 5900
Wire Wire Line
	5100 5900 5100 5550
Connection ~ 4300 4250
Connection ~ 5100 4250
Connection ~ 5100 5550
Connection ~ 4300 5550
Text Notes 3700 1400 0    50   ~ 0
Audio In L\n
Text Notes 3700 2750 0    50   ~ 0
Audio In R\n
Text Notes 6500 3950 0    50   ~ 0
Audio Out L\n
Text Notes 6500 5250 0    50   ~ 0
Audio Out R
$EndSCHEMATC

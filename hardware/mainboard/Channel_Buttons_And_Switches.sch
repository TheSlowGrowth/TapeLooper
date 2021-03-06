EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 8
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
L Switch:SW_SPST SW?
U 1 1 61F27F8E
P 10300 1650
AR Path="/61A6EF5C/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/619A998C/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F27F8E" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F27F8E" Ref="SW9"  Part="1" 
F 0 "SW9" H 10300 1885 50  0000 C CNN
F 1 "BTTN_PLAY_A" H 10300 1794 50  0000 C CNN
F 2 "ell_special:CK-D6R_Pushbutton" H 10300 1650 50  0001 C CNN
F 3 "~" H 10300 1650 50  0001 C CNN
	1    10300 1650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F27F94
P 9850 1400
AR Path="/61A6EF5C/61F27F94" Ref="R?"  Part="1" 
AR Path="/61F27F94" Ref="R?"  Part="1" 
AR Path="/619A998C/61F27F94" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27F94" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F27F94" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F27F94" Ref="R?"  Part="1" 
AR Path="/61D60973/61F27F94" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F27F94" Ref="R79"  Part="1" 
F 0 "R79" H 9920 1446 50  0000 L CNN
F 1 "10k" H 9920 1355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9780 1400 50  0001 C CNN
F 3 "~" H 9850 1400 50  0001 C CNN
	1    9850 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1550 9850 1650
Wire Wire Line
	9850 1650 10100 1650
$Comp
L power:+3.3V #PWR?
U 1 1 61F27F9C
P 9850 1150
AR Path="/61A6EF5C/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27F9C" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27F9C" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 9850 1000 50  0001 C CNN
F 1 "+3.3V" H 9865 1323 50  0000 C CNN
F 2 "" H 9850 1150 50  0001 C CNN
F 3 "" H 9850 1150 50  0001 C CNN
	1    9850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1150 9850 1250
Wire Wire Line
	9850 1650 9750 1650
Connection ~ 9850 1650
Wire Wire Line
	10500 1650 10600 1650
Wire Wire Line
	10600 1650 10600 1750
$Comp
L power:GND #PWR?
U 1 1 61F27FA7
P 10600 1750
AR Path="/61A6EF5C/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FA7" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FA7" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 10600 1500 50  0001 C CNN
F 1 "GND" H 10605 1577 50  0000 C CNN
F 2 "" H 10600 1750 50  0001 C CNN
F 3 "" H 10600 1750 50  0001 C CNN
	1    10600 1750
	1    0    0    -1  
$EndComp
Text GLabel 9750 1650 0    50   Output ~ 0
BTTN_PLAY_A
$Comp
L Device:R R?
U 1 1 61F27FAE
P 8050 2550
AR Path="/61A6EF5C/61F27FAE" Ref="R?"  Part="1" 
AR Path="/61F27FAE" Ref="R?"  Part="1" 
AR Path="/619A998C/61F27FAE" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FAE" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F27FAE" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F27FAE" Ref="R?"  Part="1" 
AR Path="/61D60973/61F27FAE" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F27FAE" Ref="R72"  Part="1" 
F 0 "R72" H 7800 2600 50  0000 L CNN
F 1 "10k" H 7800 2500 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7980 2550 50  0001 C CNN
F 3 "~" H 8050 2550 50  0001 C CNN
	1    8050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2700 8050 2800
$Comp
L power:+3.3V #PWR?
U 1 1 61F27FB5
P 8050 2300
AR Path="/61A6EF5C/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FB5" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FB5" Ref="#PWR090"  Part="1" 
F 0 "#PWR090" H 8050 2150 50  0001 C CNN
F 1 "+3.3V" H 8065 2473 50  0000 C CNN
F 2 "" H 8050 2300 50  0001 C CNN
F 3 "" H 8050 2300 50  0001 C CNN
	1    8050 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2300 8050 2400
Wire Wire Line
	8050 2800 7950 2800
Connection ~ 8050 2800
Text GLabel 7950 2800 0    50   Output ~ 0
BTTN_UP_B
$Comp
L Device:R R?
U 1 1 61F27FBF
P 8350 2550
AR Path="/61A6EF5C/61F27FBF" Ref="R?"  Part="1" 
AR Path="/61F27FBF" Ref="R?"  Part="1" 
AR Path="/619A998C/61F27FBF" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FBF" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F27FBF" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F27FBF" Ref="R?"  Part="1" 
AR Path="/61D60973/61F27FBF" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F27FBF" Ref="R76"  Part="1" 
F 0 "R76" H 8420 2596 50  0000 L CNN
F 1 "10k" H 8420 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 8280 2550 50  0001 C CNN
F 3 "~" H 8350 2550 50  0001 C CNN
	1    8350 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61F27FC5
P 8350 2300
AR Path="/61A6EF5C/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FC5" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FC5" Ref="#PWR094"  Part="1" 
F 0 "#PWR094" H 8350 2150 50  0001 C CNN
F 1 "+3.3V" H 8365 2473 50  0000 C CNN
F 2 "" H 8350 2300 50  0001 C CNN
F 3 "" H 8350 2300 50  0001 C CNN
	1    8350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 2300 8350 2400
Text GLabel 7950 3000 0    50   Output ~ 0
BTTN_DOWN_B
$Comp
L Switch:SW_SPST SW?
U 1 1 61F27FCD
P 10300 2900
AR Path="/61A6EF5C/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/619A998C/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F27FCD" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F27FCD" Ref="SW10"  Part="1" 
F 0 "SW10" H 10300 3135 50  0000 C CNN
F 1 "BTTN_PLAY_B" H 10300 3044 50  0000 C CNN
F 2 "ell_special:CK-D6R_Pushbutton" H 10300 2900 50  0001 C CNN
F 3 "~" H 10300 2900 50  0001 C CNN
	1    10300 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F27FD3
P 9850 2650
AR Path="/61A6EF5C/61F27FD3" Ref="R?"  Part="1" 
AR Path="/61F27FD3" Ref="R?"  Part="1" 
AR Path="/619A998C/61F27FD3" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FD3" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F27FD3" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F27FD3" Ref="R?"  Part="1" 
AR Path="/61D60973/61F27FD3" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F27FD3" Ref="R80"  Part="1" 
F 0 "R80" H 9920 2696 50  0000 L CNN
F 1 "10k" H 9920 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9780 2650 50  0001 C CNN
F 3 "~" H 9850 2650 50  0001 C CNN
	1    9850 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2800 9850 2900
Wire Wire Line
	9850 2900 10100 2900
$Comp
L power:+3.3V #PWR?
U 1 1 61F27FDB
P 9850 2400
AR Path="/61A6EF5C/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FDB" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FDB" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 9850 2250 50  0001 C CNN
F 1 "+3.3V" H 9865 2573 50  0000 C CNN
F 2 "" H 9850 2400 50  0001 C CNN
F 3 "" H 9850 2400 50  0001 C CNN
	1    9850 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2400 9850 2500
Wire Wire Line
	9850 2900 9750 2900
Connection ~ 9850 2900
Wire Wire Line
	10500 2900 10600 2900
Wire Wire Line
	10600 2900 10600 3000
$Comp
L power:GND #PWR?
U 1 1 61F27FE6
P 10600 3000
AR Path="/61A6EF5C/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FE6" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FE6" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 10600 2750 50  0001 C CNN
F 1 "GND" H 10605 2827 50  0000 C CNN
F 2 "" H 10600 3000 50  0001 C CNN
F 3 "" H 10600 3000 50  0001 C CNN
	1    10600 3000
	1    0    0    -1  
$EndComp
Text GLabel 9750 2900 0    50   Output ~ 0
BTTN_PLAY_B
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61F27FED
P 8650 2900
AR Path="/61D4151D/61F27FED" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F27FED" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F27FED" Ref="SW6"  Part="1" 
F 0 "SW6" H 8650 3250 50  0000 C CNN
F 1 "BTTN_SPEED_B" H 8800 3150 50  0000 C CNN
F 2 "ell_special:7105SYCQE-1P3T-SP3T-Switch" H 8650 2900 50  0001 C CNN
F 3 "~" H 8650 2900 50  0001 C CNN
	1    8650 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 2800 8450 2800
Wire Wire Line
	8450 3000 8350 3000
Wire Wire Line
	8350 3000 8350 2700
Wire Wire Line
	7950 3000 8350 3000
Connection ~ 8350 3000
Wire Wire Line
	8850 2900 8900 2900
Wire Wire Line
	8900 2900 8900 3000
$Comp
L power:GND #PWR?
U 1 1 61F27FFA
P 8900 3000
AR Path="/61A6EF5C/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F27FFA" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F27FFA" Ref="#PWR098"  Part="1" 
F 0 "#PWR098" H 8900 2750 50  0001 C CNN
F 1 "GND" H 8905 2827 50  0000 C CNN
F 2 "" H 8900 3000 50  0001 C CNN
F 3 "" H 8900 3000 50  0001 C CNN
	1    8900 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F28000
P 8050 1300
AR Path="/61A6EF5C/61F28000" Ref="R?"  Part="1" 
AR Path="/61F28000" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28000" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28000" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28000" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28000" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28000" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28000" Ref="R71"  Part="1" 
F 0 "R71" H 7800 1350 50  0000 L CNN
F 1 "10k" H 7800 1250 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7980 1300 50  0001 C CNN
F 3 "~" H 8050 1300 50  0001 C CNN
	1    8050 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1450 8050 1550
$Comp
L power:+3.3V #PWR?
U 1 1 61F28007
P 8050 1050
AR Path="/61A6EF5C/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F28007" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F28007" Ref="#PWR089"  Part="1" 
F 0 "#PWR089" H 8050 900 50  0001 C CNN
F 1 "+3.3V" H 8065 1223 50  0000 C CNN
F 2 "" H 8050 1050 50  0001 C CNN
F 3 "" H 8050 1050 50  0001 C CNN
	1    8050 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1050 8050 1150
Wire Wire Line
	8050 1550 7950 1550
Connection ~ 8050 1550
Text GLabel 7950 1550 0    50   Output ~ 0
BTTN_UP_A
$Comp
L Device:R R?
U 1 1 61F28011
P 8350 1300
AR Path="/61A6EF5C/61F28011" Ref="R?"  Part="1" 
AR Path="/61F28011" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28011" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28011" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28011" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28011" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28011" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28011" Ref="R75"  Part="1" 
F 0 "R75" H 8420 1346 50  0000 L CNN
F 1 "10k" H 8420 1255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 8280 1300 50  0001 C CNN
F 3 "~" H 8350 1300 50  0001 C CNN
	1    8350 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61F28017
P 8350 1050
AR Path="/61A6EF5C/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F28017" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F28017" Ref="#PWR093"  Part="1" 
F 0 "#PWR093" H 8350 900 50  0001 C CNN
F 1 "+3.3V" H 8365 1223 50  0000 C CNN
F 2 "" H 8350 1050 50  0001 C CNN
F 3 "" H 8350 1050 50  0001 C CNN
	1    8350 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1050 8350 1150
Text GLabel 7950 1750 0    50   Output ~ 0
BTTN_DOWN_A
$Comp
L Switch:SW_SPDT_MSM SW5
U 1 1 61F2801F
P 8650 1650
F 0 "SW5" H 8650 2000 50  0000 C CNN
F 1 "BTTN_SPEED_A" H 8800 1900 50  0000 C CNN
F 2 "ell_special:7105SYCQE-1P3T-SP3T-Switch" H 8650 1650 50  0001 C CNN
F 3 "~" H 8650 1650 50  0001 C CNN
	1    8650 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 1550 8450 1550
Wire Wire Line
	8450 1750 8350 1750
Wire Wire Line
	8350 1750 8350 1450
Wire Wire Line
	7950 1750 8350 1750
Connection ~ 8350 1750
Wire Wire Line
	8850 1650 8900 1650
Wire Wire Line
	8900 1650 8900 1750
$Comp
L power:GND #PWR?
U 1 1 61F2802C
P 8900 1750
AR Path="/61A6EF5C/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F2802C" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F2802C" Ref="#PWR097"  Part="1" 
F 0 "#PWR097" H 8900 1500 50  0001 C CNN
F 1 "GND" H 8905 1577 50  0000 C CNN
F 2 "" H 8900 1750 50  0001 C CNN
F 3 "" H 8900 1750 50  0001 C CNN
	1    8900 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW?
U 1 1 61F28032
P 10300 4150
AR Path="/61A6EF5C/61F28032" Ref="SW?"  Part="1" 
AR Path="/61F28032" Ref="SW?"  Part="1" 
AR Path="/619A998C/61F28032" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28032" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61F28032" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61F28032" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F28032" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F28032" Ref="SW11"  Part="1" 
F 0 "SW11" H 10300 4385 50  0000 C CNN
F 1 "BTTN_PLAY_C" H 10300 4294 50  0000 C CNN
F 2 "ell_special:CK-D6R_Pushbutton" H 10300 4150 50  0001 C CNN
F 3 "~" H 10300 4150 50  0001 C CNN
	1    10300 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F28038
P 9850 3900
AR Path="/61A6EF5C/61F28038" Ref="R?"  Part="1" 
AR Path="/61F28038" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28038" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28038" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28038" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28038" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28038" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28038" Ref="R81"  Part="1" 
F 0 "R81" H 9920 3946 50  0000 L CNN
F 1 "10k" H 9920 3855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9780 3900 50  0001 C CNN
F 3 "~" H 9850 3900 50  0001 C CNN
	1    9850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4050 9850 4150
Wire Wire Line
	9850 4150 10100 4150
$Comp
L power:+3.3V #PWR?
U 1 1 61F28040
P 9850 3650
AR Path="/61A6EF5C/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F28040" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F28040" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 9850 3500 50  0001 C CNN
F 1 "+3.3V" H 9865 3823 50  0000 C CNN
F 2 "" H 9850 3650 50  0001 C CNN
F 3 "" H 9850 3650 50  0001 C CNN
	1    9850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 3650 9850 3750
Wire Wire Line
	9850 4150 9750 4150
Connection ~ 9850 4150
Wire Wire Line
	10500 4150 10600 4150
Wire Wire Line
	10600 4150 10600 4250
$Comp
L power:GND #PWR?
U 1 1 61F2804B
P 10600 4250
AR Path="/61A6EF5C/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F2804B" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F2804B" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 10600 4000 50  0001 C CNN
F 1 "GND" H 10605 4077 50  0000 C CNN
F 2 "" H 10600 4250 50  0001 C CNN
F 3 "" H 10600 4250 50  0001 C CNN
	1    10600 4250
	1    0    0    -1  
$EndComp
Text GLabel 9750 4150 0    50   Output ~ 0
BTTN_PLAY_C
$Comp
L Device:R R?
U 1 1 61F28052
P 8050 5050
AR Path="/61A6EF5C/61F28052" Ref="R?"  Part="1" 
AR Path="/61F28052" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28052" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28052" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28052" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28052" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28052" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28052" Ref="R74"  Part="1" 
F 0 "R74" H 7800 5100 50  0000 L CNN
F 1 "10k" H 7800 5000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7980 5050 50  0001 C CNN
F 3 "~" H 8050 5050 50  0001 C CNN
	1    8050 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 5200 8050 5300
$Comp
L power:+3.3V #PWR?
U 1 1 61F28059
P 8050 4800
AR Path="/61A6EF5C/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F28059" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F28059" Ref="#PWR092"  Part="1" 
F 0 "#PWR092" H 8050 4650 50  0001 C CNN
F 1 "+3.3V" H 8065 4973 50  0000 C CNN
F 2 "" H 8050 4800 50  0001 C CNN
F 3 "" H 8050 4800 50  0001 C CNN
	1    8050 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4800 8050 4900
Wire Wire Line
	8050 5300 7950 5300
Connection ~ 8050 5300
Text GLabel 7950 5300 0    50   Output ~ 0
BTTN_UP_D
$Comp
L Device:R R?
U 1 1 61F28063
P 8350 5050
AR Path="/61A6EF5C/61F28063" Ref="R?"  Part="1" 
AR Path="/61F28063" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28063" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28063" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28063" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28063" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28063" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28063" Ref="R78"  Part="1" 
F 0 "R78" H 8420 5096 50  0000 L CNN
F 1 "10k" H 8420 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 8280 5050 50  0001 C CNN
F 3 "~" H 8350 5050 50  0001 C CNN
	1    8350 5050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61F28069
P 8350 4800
AR Path="/61A6EF5C/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F28069" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F28069" Ref="#PWR096"  Part="1" 
F 0 "#PWR096" H 8350 4650 50  0001 C CNN
F 1 "+3.3V" H 8365 4973 50  0000 C CNN
F 2 "" H 8350 4800 50  0001 C CNN
F 3 "" H 8350 4800 50  0001 C CNN
	1    8350 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 4800 8350 4900
Text GLabel 7950 5500 0    50   Output ~ 0
BTTN_DOWN_D
$Comp
L Switch:SW_SPST SW?
U 1 1 61F28071
P 10300 5400
AR Path="/61A6EF5C/61F28071" Ref="SW?"  Part="1" 
AR Path="/61F28071" Ref="SW?"  Part="1" 
AR Path="/619A998C/61F28071" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28071" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61F28071" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61F28071" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F28071" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F28071" Ref="SW12"  Part="1" 
F 0 "SW12" H 10300 5635 50  0000 C CNN
F 1 "BTTN_PLAY_D" H 10300 5544 50  0000 C CNN
F 2 "ell_special:CK-D6R_Pushbutton" H 10300 5400 50  0001 C CNN
F 3 "~" H 10300 5400 50  0001 C CNN
	1    10300 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F28077
P 9850 5150
AR Path="/61A6EF5C/61F28077" Ref="R?"  Part="1" 
AR Path="/61F28077" Ref="R?"  Part="1" 
AR Path="/619A998C/61F28077" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F28077" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F28077" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F28077" Ref="R?"  Part="1" 
AR Path="/61D60973/61F28077" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F28077" Ref="R82"  Part="1" 
F 0 "R82" H 9920 5196 50  0000 L CNN
F 1 "10k" H 9920 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9780 5150 50  0001 C CNN
F 3 "~" H 9850 5150 50  0001 C CNN
	1    9850 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 5300 9850 5400
Wire Wire Line
	9850 5400 10100 5400
$Comp
L power:+3.3V #PWR?
U 1 1 61F2807F
P 9850 4900
AR Path="/61A6EF5C/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F2807F" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F2807F" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 9850 4750 50  0001 C CNN
F 1 "+3.3V" H 9865 5073 50  0000 C CNN
F 2 "" H 9850 4900 50  0001 C CNN
F 3 "" H 9850 4900 50  0001 C CNN
	1    9850 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4900 9850 5000
Wire Wire Line
	9850 5400 9750 5400
Connection ~ 9850 5400
Wire Wire Line
	10500 5400 10600 5400
Wire Wire Line
	10600 5400 10600 5500
$Comp
L power:GND #PWR?
U 1 1 61F2808A
P 10600 5500
AR Path="/61A6EF5C/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F2808A" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F2808A" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 10600 5250 50  0001 C CNN
F 1 "GND" H 10605 5327 50  0000 C CNN
F 2 "" H 10600 5500 50  0001 C CNN
F 3 "" H 10600 5500 50  0001 C CNN
	1    10600 5500
	1    0    0    -1  
$EndComp
Text GLabel 9750 5400 0    50   Output ~ 0
BTTN_PLAY_D
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61F28091
P 8650 5400
AR Path="/61D4151D/61F28091" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F28091" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F28091" Ref="SW8"  Part="1" 
F 0 "SW8" H 8650 5750 50  0000 C CNN
F 1 "BTTN_SPEED_D" H 8800 5650 50  0000 C CNN
F 2 "ell_special:7105SYCQE-1P3T-SP3T-Switch" H 8650 5400 50  0001 C CNN
F 3 "~" H 8650 5400 50  0001 C CNN
	1    8650 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 5300 8450 5300
Wire Wire Line
	8450 5500 8350 5500
Wire Wire Line
	8350 5500 8350 5200
Wire Wire Line
	7950 5500 8350 5500
Connection ~ 8350 5500
Wire Wire Line
	8850 5400 8900 5400
Wire Wire Line
	8900 5400 8900 5500
$Comp
L power:GND #PWR?
U 1 1 61F2809E
P 8900 5500
AR Path="/61A6EF5C/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F2809E" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F2809E" Ref="#PWR0100"  Part="1" 
F 0 "#PWR0100" H 8900 5250 50  0001 C CNN
F 1 "GND" H 8905 5327 50  0000 C CNN
F 2 "" H 8900 5500 50  0001 C CNN
F 3 "" H 8900 5500 50  0001 C CNN
	1    8900 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61F280A4
P 8050 3800
AR Path="/61A6EF5C/61F280A4" Ref="R?"  Part="1" 
AR Path="/61F280A4" Ref="R?"  Part="1" 
AR Path="/619A998C/61F280A4" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F280A4" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F280A4" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F280A4" Ref="R?"  Part="1" 
AR Path="/61D60973/61F280A4" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F280A4" Ref="R73"  Part="1" 
F 0 "R73" H 7800 3850 50  0000 L CNN
F 1 "10k" H 7800 3750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7980 3800 50  0001 C CNN
F 3 "~" H 8050 3800 50  0001 C CNN
	1    8050 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3950 8050 4050
$Comp
L power:+3.3V #PWR?
U 1 1 61F280AB
P 8050 3550
AR Path="/61A6EF5C/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F280AB" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F280AB" Ref="#PWR091"  Part="1" 
F 0 "#PWR091" H 8050 3400 50  0001 C CNN
F 1 "+3.3V" H 8065 3723 50  0000 C CNN
F 2 "" H 8050 3550 50  0001 C CNN
F 3 "" H 8050 3550 50  0001 C CNN
	1    8050 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3550 8050 3650
Wire Wire Line
	8050 4050 7950 4050
Connection ~ 8050 4050
Text GLabel 7950 4050 0    50   Output ~ 0
BTTN_UP_C
$Comp
L Device:R R?
U 1 1 61F280B5
P 8350 3800
AR Path="/61A6EF5C/61F280B5" Ref="R?"  Part="1" 
AR Path="/61F280B5" Ref="R?"  Part="1" 
AR Path="/619A998C/61F280B5" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61F280B5" Ref="R?"  Part="1" 
AR Path="/61C541EC/61F280B5" Ref="R?"  Part="1" 
AR Path="/61D4151D/61F280B5" Ref="R?"  Part="1" 
AR Path="/61D60973/61F280B5" Ref="R?"  Part="1" 
AR Path="/61F0493E/61F280B5" Ref="R77"  Part="1" 
F 0 "R77" H 8420 3846 50  0000 L CNN
F 1 "10k" H 8420 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 8280 3800 50  0001 C CNN
F 3 "~" H 8350 3800 50  0001 C CNN
	1    8350 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61F280BB
P 8350 3550
AR Path="/61A6EF5C/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F280BB" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F280BB" Ref="#PWR095"  Part="1" 
F 0 "#PWR095" H 8350 3400 50  0001 C CNN
F 1 "+3.3V" H 8365 3723 50  0000 C CNN
F 2 "" H 8350 3550 50  0001 C CNN
F 3 "" H 8350 3550 50  0001 C CNN
	1    8350 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3550 8350 3650
Text GLabel 7950 4250 0    50   Output ~ 0
BTTN_DOWN_C
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61F280C3
P 8650 4150
AR Path="/61D4151D/61F280C3" Ref="SW?"  Part="1" 
AR Path="/61D60973/61F280C3" Ref="SW?"  Part="1" 
AR Path="/61F0493E/61F280C3" Ref="SW7"  Part="1" 
F 0 "SW7" H 8650 4500 50  0000 C CNN
F 1 "BTTN_SPEED_C" H 8800 4400 50  0000 C CNN
F 2 "ell_special:7105SYCQE-1P3T-SP3T-Switch" H 8650 4150 50  0001 C CNN
F 3 "~" H 8650 4150 50  0001 C CNN
	1    8650 4150
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 4050 8450 4050
Wire Wire Line
	8450 4250 8350 4250
Wire Wire Line
	8350 4250 8350 3950
Wire Wire Line
	7950 4250 8350 4250
Connection ~ 8350 4250
Wire Wire Line
	8850 4150 8900 4150
Wire Wire Line
	8900 4150 8900 4250
$Comp
L power:GND #PWR?
U 1 1 61F280D0
P 8900 4250
AR Path="/61A6EF5C/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61F280D0" Ref="#PWR?"  Part="1" 
AR Path="/61F0493E/61F280D0" Ref="#PWR099"  Part="1" 
F 0 "#PWR099" H 8900 4000 50  0001 C CNN
F 1 "GND" H 8905 4077 50  0000 C CNN
F 2 "" H 8900 4250 50  0001 C CNN
F 3 "" H 8900 4250 50  0001 C CNN
	1    8900 4250
	1    0    0    -1  
$EndComp
$EndSCHEMATC

EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
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
L Driver_LED:PCA9685PW U?
U 1 1 61D9F718
P 1900 2600
AR Path="/61A6EF5C/61D9F718" Ref="U?"  Part="1" 
AR Path="/61D9F718" Ref="U?"  Part="1" 
AR Path="/619A998C/61D9F718" Ref="U?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F718" Ref="U?"  Part="1" 
AR Path="/61C541EA/61D9F718" Ref="U?"  Part="1" 
AR Path="/61D4151D/61D9F718" Ref="U?"  Part="1" 
AR Path="/61D60973/61D9F718" Ref="U?"  Part="1" 
F 0 "U?" H 2300 3450 50  0000 C CNN
F 1 "PCA9685PW" H 1600 3450 50  0000 C CNN
F 2 "Package_SO:TSSOP-28_4.4x9.7mm_P0.65mm" H 1925 1625 50  0001 L CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCA9685.pdf" H 1500 3300 50  0001 C CNN
	1    1900 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F71E
P 1900 1600
AR Path="/61A6EF5C/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F71E" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F71E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 1450 50  0001 C CNN
F 1 "+3.3V" H 1915 1773 50  0000 C CNN
F 2 "" H 1900 1600 50  0001 C CNN
F 3 "" H 1900 1600 50  0001 C CNN
	1    1900 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D9F724
P 1900 3700
AR Path="/61A6EF5C/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F724" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F724" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 3450 50  0001 C CNN
F 1 "GND" H 1905 3527 50  0000 C CNN
F 2 "" H 1900 3700 50  0001 C CNN
F 3 "" H 1900 3700 50  0001 C CNN
	1    1900 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D9F72A
P 1100 3400
AR Path="/61A6EF5C/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F72A" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F72A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1100 3150 50  0001 C CNN
F 1 "GND" H 1105 3227 50  0000 C CNN
F 2 "" H 1100 3400 50  0001 C CNN
F 3 "" H 1100 3400 50  0001 C CNN
	1    1100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2900 1100 2900
Connection ~ 1100 2900
Wire Wire Line
	1100 2900 1100 3000
Wire Wire Line
	1100 3000 1200 3000
Connection ~ 1100 3000
Wire Wire Line
	1100 3000 1100 3100
Wire Wire Line
	1200 3100 1100 3100
Connection ~ 1100 3100
Wire Wire Line
	1100 3100 1100 3200
Wire Wire Line
	1100 3200 1200 3200
Connection ~ 1100 3200
Wire Wire Line
	1100 3200 1100 3300
Wire Wire Line
	1200 3300 1100 3300
Connection ~ 1100 3300
Wire Wire Line
	1100 3300 1100 3400
Text GLabel 900  2000 0    50   Input ~ 0
LED_SDA
Text GLabel 900  1900 0    50   Input ~ 0
LED_SCL
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F741
P 2850 1250
AR Path="/61A6EF5C/61D9F741" Ref="D?"  Part="1" 
AR Path="/61D9F741" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F741" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F741" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F741" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F741" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F741" Ref="D?"  Part="1" 
F 0 "D?" V 2650 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 3450 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 2900 1250 50  0001 C CNN
F 3 "~" H 2900 1250 50  0001 C CNN
	1    2850 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F747
P 2750 1700
AR Path="/61A6EF5C/61D9F747" Ref="R?"  Part="1" 
AR Path="/61D9F747" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F747" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F747" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F747" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F747" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F747" Ref="R?"  Part="1" 
F 0 "R?" H 2820 1746 50  0000 L CNN
F 1 "330" H 2820 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2680 1700 50  0001 C CNN
F 3 "~" H 2750 1700 50  0001 C CNN
	1    2750 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F74D
P 2950 1700
AR Path="/61A6EF5C/61D9F74D" Ref="R?"  Part="1" 
AR Path="/61D9F74D" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F74D" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F74D" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F74D" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F74D" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F74D" Ref="R?"  Part="1" 
F 0 "R?" H 3020 1746 50  0000 L CNN
F 1 "330" H 3020 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2880 1700 50  0001 C CNN
F 3 "~" H 2950 1700 50  0001 C CNN
	1    2950 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 950  2850 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F754
P 2850 850
AR Path="/61A6EF5C/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F754" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F754" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2850 700 50  0001 C CNN
F 1 "+3.3V" H 2865 1023 50  0000 C CNN
F 2 "" H 2850 850 50  0001 C CNN
F 3 "" H 2850 850 50  0001 C CNN
	1    2850 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F75A
P 3400 1250
AR Path="/61A6EF5C/61D9F75A" Ref="D?"  Part="1" 
AR Path="/61D9F75A" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F75A" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F75A" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F75A" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F75A" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F75A" Ref="D?"  Part="1" 
F 0 "D?" V 3200 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 4000 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 3450 1250 50  0001 C CNN
F 3 "~" H 3450 1250 50  0001 C CNN
	1    3400 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F760
P 3300 1700
AR Path="/61A6EF5C/61D9F760" Ref="R?"  Part="1" 
AR Path="/61D9F760" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F760" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F760" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F760" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F760" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F760" Ref="R?"  Part="1" 
F 0 "R?" H 3370 1746 50  0000 L CNN
F 1 "330" H 3370 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3230 1700 50  0001 C CNN
F 3 "~" H 3300 1700 50  0001 C CNN
	1    3300 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F766
P 3500 1700
AR Path="/61A6EF5C/61D9F766" Ref="R?"  Part="1" 
AR Path="/61D9F766" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F766" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F766" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F766" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F766" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F766" Ref="R?"  Part="1" 
F 0 "R?" H 3570 1746 50  0000 L CNN
F 1 "330" H 3570 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3430 1700 50  0001 C CNN
F 3 "~" H 3500 1700 50  0001 C CNN
	1    3500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 950  3400 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F76D
P 3400 850
AR Path="/61A6EF5C/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F76D" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F76D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3400 700 50  0001 C CNN
F 1 "+3.3V" H 3415 1023 50  0000 C CNN
F 2 "" H 3400 850 50  0001 C CNN
F 3 "" H 3400 850 50  0001 C CNN
	1    3400 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F773
P 4200 1250
AR Path="/61A6EF5C/61D9F773" Ref="D?"  Part="1" 
AR Path="/61D9F773" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F773" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F773" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F773" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F773" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F773" Ref="D?"  Part="1" 
F 0 "D?" V 4000 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 4800 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 4250 1250 50  0001 C CNN
F 3 "~" H 4250 1250 50  0001 C CNN
	1    4200 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F779
P 4100 1700
AR Path="/61A6EF5C/61D9F779" Ref="R?"  Part="1" 
AR Path="/61D9F779" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F779" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F779" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F779" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F779" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F779" Ref="R?"  Part="1" 
F 0 "R?" H 4170 1746 50  0000 L CNN
F 1 "330" H 4170 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4030 1700 50  0001 C CNN
F 3 "~" H 4100 1700 50  0001 C CNN
	1    4100 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F77F
P 4300 1700
AR Path="/61A6EF5C/61D9F77F" Ref="R?"  Part="1" 
AR Path="/61D9F77F" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F77F" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F77F" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F77F" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F77F" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F77F" Ref="R?"  Part="1" 
F 0 "R?" H 4370 1746 50  0000 L CNN
F 1 "330" H 4370 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4230 1700 50  0001 C CNN
F 3 "~" H 4300 1700 50  0001 C CNN
	1    4300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 950  4200 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F786
P 4200 850
AR Path="/61A6EF5C/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F786" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F786" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4200 700 50  0001 C CNN
F 1 "+3.3V" H 4215 1023 50  0000 C CNN
F 2 "" H 4200 850 50  0001 C CNN
F 3 "" H 4200 850 50  0001 C CNN
	1    4200 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F78C
P 4750 1250
AR Path="/61A6EF5C/61D9F78C" Ref="D?"  Part="1" 
AR Path="/61D9F78C" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F78C" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F78C" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F78C" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F78C" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F78C" Ref="D?"  Part="1" 
F 0 "D?" V 4550 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 5350 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 4800 1250 50  0001 C CNN
F 3 "~" H 4800 1250 50  0001 C CNN
	1    4750 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F792
P 4650 1700
AR Path="/61A6EF5C/61D9F792" Ref="R?"  Part="1" 
AR Path="/61D9F792" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F792" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F792" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F792" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F792" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F792" Ref="R?"  Part="1" 
F 0 "R?" H 4720 1746 50  0000 L CNN
F 1 "330" H 4720 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4580 1700 50  0001 C CNN
F 3 "~" H 4650 1700 50  0001 C CNN
	1    4650 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F798
P 4850 1700
AR Path="/61A6EF5C/61D9F798" Ref="R?"  Part="1" 
AR Path="/61D9F798" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F798" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F798" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F798" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F798" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F798" Ref="R?"  Part="1" 
F 0 "R?" H 4920 1746 50  0000 L CNN
F 1 "330" H 4920 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4780 1700 50  0001 C CNN
F 3 "~" H 4850 1700 50  0001 C CNN
	1    4850 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 950  4750 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F79F
P 4750 850
AR Path="/61A6EF5C/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F79F" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F79F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4750 700 50  0001 C CNN
F 1 "+3.3V" H 4765 1023 50  0000 C CNN
F 2 "" H 4750 850 50  0001 C CNN
F 3 "" H 4750 850 50  0001 C CNN
	1    4750 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1850 2750 1900
Wire Wire Line
	2750 1900 2600 1900
Wire Wire Line
	2600 2000 2950 2000
Wire Wire Line
	2950 2000 2950 1850
Wire Wire Line
	2600 2100 3300 2100
Wire Wire Line
	3300 2100 3300 1850
Wire Wire Line
	3500 1850 3500 2200
Wire Wire Line
	3500 2200 2600 2200
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F7AD
P 5500 1250
AR Path="/61A6EF5C/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F7AD" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F7AD" Ref="D?"  Part="1" 
F 0 "D?" V 5300 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 6100 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 5550 1250 50  0001 C CNN
F 3 "~" H 5550 1250 50  0001 C CNN
	1    5500 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F7B3
P 5400 1700
AR Path="/61A6EF5C/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F7B3" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F7B3" Ref="R?"  Part="1" 
F 0 "R?" H 5470 1746 50  0000 L CNN
F 1 "330" H 5470 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5330 1700 50  0001 C CNN
F 3 "~" H 5400 1700 50  0001 C CNN
	1    5400 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F7B9
P 5600 1700
AR Path="/61A6EF5C/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F7B9" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F7B9" Ref="R?"  Part="1" 
F 0 "R?" H 5670 1746 50  0000 L CNN
F 1 "330" H 5670 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5530 1700 50  0001 C CNN
F 3 "~" H 5600 1700 50  0001 C CNN
	1    5600 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 950  5500 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F7C0
P 5500 850
AR Path="/61A6EF5C/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F7C0" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F7C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5500 700 50  0001 C CNN
F 1 "+3.3V" H 5515 1023 50  0000 C CNN
F 2 "" H 5500 850 50  0001 C CNN
F 3 "" H 5500 850 50  0001 C CNN
	1    5500 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 1850 5400 2700
Wire Wire Line
	5600 1850 5600 2800
Text GLabel 900  2200 0    50   Input ~ 0
LED_OE
Wire Wire Line
	4300 2400 2600 2400
Wire Wire Line
	2600 2300 4100 2300
$Comp
L Device:C C?
U 1 1 61D9F7CB
P 1050 1200
AR Path="/5EDCCD5B/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/5EDCCDFD/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/61A6EF5C/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/619A998C/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/61C541EA/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/61D4151D/61D9F7CB" Ref="C?"  Part="1" 
AR Path="/61D60973/61D9F7CB" Ref="C?"  Part="1" 
F 0 "C?" H 1165 1246 50  0000 L CNN
F 1 "10u" H 1165 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1088 1050 50  0001 C CNN
F 3 "" H 1050 1200 50  0001 C CNN
	1    1050 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61D9F7D1
P 1450 1200
AR Path="/5EDCCD5B/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/5EDCCDFD/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/61A6EF5C/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/619A998C/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/61C541EA/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/61D4151D/61D9F7D1" Ref="C?"  Part="1" 
AR Path="/61D60973/61D9F7D1" Ref="C?"  Part="1" 
F 0 "C?" H 1565 1246 50  0000 L CNN
F 1 "100n" H 1565 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1488 1050 50  0001 C CNN
F 3 "" H 1450 1200 50  0001 C CNN
	1    1450 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D9F7D7
P 1450 1350
AR Path="/61A6EF5C/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F7D7" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F7D7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1450 1100 50  0001 C CNN
F 1 "GND" H 1455 1177 50  0000 C CNN
F 2 "" H 1450 1350 50  0001 C CNN
F 3 "" H 1450 1350 50  0001 C CNN
	1    1450 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D9F7DD
P 1050 1350
AR Path="/61A6EF5C/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F7DD" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F7DD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1050 1100 50  0001 C CNN
F 1 "GND" H 1055 1177 50  0000 C CNN
F 2 "" H 1050 1350 50  0001 C CNN
F 3 "" H 1050 1350 50  0001 C CNN
	1    1050 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F7E3
P 1450 1050
AR Path="/61A6EF5C/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F7E3" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F7E3" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1450 900 50  0001 C CNN
F 1 "+3.3V" H 1465 1223 50  0000 C CNN
F 2 "" H 1450 1050 50  0001 C CNN
F 3 "" H 1450 1050 50  0001 C CNN
	1    1450 1050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F7E9
P 1050 1050
AR Path="/61A6EF5C/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F7E9" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F7E9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1050 900 50  0001 C CNN
F 1 "+3.3V" H 1065 1223 50  0000 C CNN
F 2 "" H 1050 1050 50  0001 C CNN
F 3 "" H 1050 1050 50  0001 C CNN
	1    1050 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1850 4100 2300
Wire Wire Line
	2600 2500 4650 2500
Wire Wire Line
	4300 1850 4300 2400
Wire Wire Line
	2600 2600 4850 2600
Wire Wire Line
	4650 1850 4650 2500
Wire Wire Line
	2600 2700 5400 2700
Wire Wire Line
	4850 1850 4850 2600
Wire Wire Line
	2600 2800 5600 2800
Wire Wire Line
	1100 2100 1200 2100
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F7F8
P 6050 1250
AR Path="/61A6EF5C/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F7F8" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F7F8" Ref="D?"  Part="1" 
F 0 "D?" V 5850 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 6650 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 6100 1250 50  0001 C CNN
F 3 "~" H 6100 1250 50  0001 C CNN
	1    6050 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F7FE
P 5950 1700
AR Path="/61A6EF5C/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F7FE" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F7FE" Ref="R?"  Part="1" 
F 0 "R?" H 6020 1746 50  0000 L CNN
F 1 "330" H 6020 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5880 1700 50  0001 C CNN
F 3 "~" H 5950 1700 50  0001 C CNN
	1    5950 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F804
P 6150 1700
AR Path="/61A6EF5C/61D9F804" Ref="R?"  Part="1" 
AR Path="/61D9F804" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F804" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F804" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F804" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F804" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F804" Ref="R?"  Part="1" 
F 0 "R?" H 6220 1746 50  0000 L CNN
F 1 "330" H 6220 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6080 1700 50  0001 C CNN
F 3 "~" H 6150 1700 50  0001 C CNN
	1    6150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 950  6050 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F80B
P 6050 850
AR Path="/61A6EF5C/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F80B" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F80B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6050 700 50  0001 C CNN
F 1 "+3.3V" H 6065 1023 50  0000 C CNN
F 2 "" H 6050 850 50  0001 C CNN
F 3 "" H 6050 850 50  0001 C CNN
	1    6050 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1850 5950 2900
Wire Wire Line
	6150 1850 6150 3000
Wire Wire Line
	2600 2900 5950 2900
Wire Wire Line
	2600 3000 6150 3000
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F815
P 6600 1250
AR Path="/61A6EF5C/61D9F815" Ref="D?"  Part="1" 
AR Path="/61D9F815" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F815" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F815" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F815" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F815" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F815" Ref="D?"  Part="1" 
F 0 "D?" V 6400 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 7200 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 6650 1250 50  0001 C CNN
F 3 "~" H 6650 1250 50  0001 C CNN
	1    6600 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F81B
P 6500 1700
AR Path="/61A6EF5C/61D9F81B" Ref="R?"  Part="1" 
AR Path="/61D9F81B" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F81B" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F81B" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F81B" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F81B" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F81B" Ref="R?"  Part="1" 
F 0 "R?" H 6570 1746 50  0000 L CNN
F 1 "330" H 6570 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6430 1700 50  0001 C CNN
F 3 "~" H 6500 1700 50  0001 C CNN
	1    6500 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F821
P 6700 1700
AR Path="/61A6EF5C/61D9F821" Ref="R?"  Part="1" 
AR Path="/61D9F821" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F821" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F821" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F821" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F821" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F821" Ref="R?"  Part="1" 
F 0 "R?" H 6770 1746 50  0000 L CNN
F 1 "330" H 6770 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6630 1700 50  0001 C CNN
F 3 "~" H 6700 1700 50  0001 C CNN
	1    6700 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 950  6600 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F828
P 6600 850
AR Path="/61A6EF5C/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F828" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F828" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6600 700 50  0001 C CNN
F 1 "+3.3V" H 6615 1023 50  0000 C CNN
F 2 "" H 6600 850 50  0001 C CNN
F 3 "" H 6600 850 50  0001 C CNN
	1    6600 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1850 6500 3100
Wire Wire Line
	6700 1850 6700 3200
Wire Wire Line
	2600 3100 6500 3100
Wire Wire Line
	2600 3200 6700 3200
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61D9F832
P 7150 1250
AR Path="/61A6EF5C/61D9F832" Ref="D?"  Part="1" 
AR Path="/61D9F832" Ref="D?"  Part="1" 
AR Path="/619A998C/61D9F832" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F832" Ref="D?"  Part="1" 
AR Path="/61C541EA/61D9F832" Ref="D?"  Part="1" 
AR Path="/61D4151D/61D9F832" Ref="D?"  Part="1" 
AR Path="/61D60973/61D9F832" Ref="D?"  Part="1" 
F 0 "D?" V 6950 1400 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 7750 1250 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 7200 1250 50  0001 C CNN
F 3 "~" H 7200 1250 50  0001 C CNN
	1    7150 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F838
P 7050 1700
AR Path="/61A6EF5C/61D9F838" Ref="R?"  Part="1" 
AR Path="/61D9F838" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F838" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F838" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F838" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F838" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F838" Ref="R?"  Part="1" 
F 0 "R?" H 7120 1746 50  0000 L CNN
F 1 "330" H 7120 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6980 1700 50  0001 C CNN
F 3 "~" H 7050 1700 50  0001 C CNN
	1    7050 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61D9F83E
P 7250 1700
AR Path="/61A6EF5C/61D9F83E" Ref="R?"  Part="1" 
AR Path="/61D9F83E" Ref="R?"  Part="1" 
AR Path="/619A998C/61D9F83E" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F83E" Ref="R?"  Part="1" 
AR Path="/61C541EA/61D9F83E" Ref="R?"  Part="1" 
AR Path="/61D4151D/61D9F83E" Ref="R?"  Part="1" 
AR Path="/61D60973/61D9F83E" Ref="R?"  Part="1" 
F 0 "R?" H 7320 1746 50  0000 L CNN
F 1 "330" H 7320 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7180 1700 50  0001 C CNN
F 3 "~" H 7250 1700 50  0001 C CNN
	1    7250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 950  7150 850 
$Comp
L power:+3.3V #PWR?
U 1 1 61D9F845
P 7150 850
AR Path="/61A6EF5C/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61D9F845" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61D9F845" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7150 700 50  0001 C CNN
F 1 "+3.3V" H 7165 1023 50  0000 C CNN
F 2 "" H 7150 850 50  0001 C CNN
F 3 "" H 7150 850 50  0001 C CNN
	1    7150 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 1850 7050 3300
Wire Wire Line
	7250 1850 7250 3400
Wire Wire Line
	2600 3300 7050 3300
Wire Wire Line
	2600 3400 7250 3400
Wire Wire Line
	900  2200 1200 2200
Wire Wire Line
	900  1900 1200 1900
Wire Wire Line
	900  2000 1200 2000
Wire Wire Line
	1100 2100 1100 2800
$Comp
L Driver_LED:PCA9685PW U?
U 1 1 61E60B7B
P 1900 5550
AR Path="/61A6EF5C/61E60B7B" Ref="U?"  Part="1" 
AR Path="/61E60B7B" Ref="U?"  Part="1" 
AR Path="/619A998C/61E60B7B" Ref="U?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60B7B" Ref="U?"  Part="1" 
AR Path="/61C541EA/61E60B7B" Ref="U?"  Part="1" 
AR Path="/61D4151D/61E60B7B" Ref="U?"  Part="1" 
AR Path="/61D60973/61E60B7B" Ref="U?"  Part="1" 
F 0 "U?" H 2300 6400 50  0000 C CNN
F 1 "PCA9685PW" H 1600 6400 50  0000 C CNN
F 2 "Package_SO:TSSOP-28_4.4x9.7mm_P0.65mm" H 1925 4575 50  0001 L CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCA9685.pdf" H 1500 6250 50  0001 C CNN
	1    1900 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61E60B81
P 1900 4550
AR Path="/61A6EF5C/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60B81" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60B81" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 4400 50  0001 C CNN
F 1 "+3.3V" H 1915 4723 50  0000 C CNN
F 2 "" H 1900 4550 50  0001 C CNN
F 3 "" H 1900 4550 50  0001 C CNN
	1    1900 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61E60B87
P 1900 6650
AR Path="/61A6EF5C/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60B87" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60B87" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 6400 50  0001 C CNN
F 1 "GND" H 1905 6477 50  0000 C CNN
F 2 "" H 1900 6650 50  0001 C CNN
F 3 "" H 1900 6650 50  0001 C CNN
	1    1900 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61E60B8D
P 1100 6350
AR Path="/61A6EF5C/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60B8D" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60B8D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1100 6100 50  0001 C CNN
F 1 "GND" H 1105 6177 50  0000 C CNN
F 2 "" H 1100 6350 50  0001 C CNN
F 3 "" H 1100 6350 50  0001 C CNN
	1    1100 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 5850 1100 5850
Connection ~ 1100 5850
Wire Wire Line
	1100 5850 1100 5950
Wire Wire Line
	1100 5950 1200 5950
Connection ~ 1100 5950
Wire Wire Line
	1100 5950 1100 6050
Wire Wire Line
	1200 6050 1100 6050
Connection ~ 1100 6050
Wire Wire Line
	1100 6050 1100 6150
Wire Wire Line
	1100 6150 1200 6150
Connection ~ 1100 6150
Wire Wire Line
	1100 6150 1100 6250
Wire Wire Line
	1200 6250 1100 6250
Connection ~ 1100 6250
Wire Wire Line
	1100 6250 1100 6350
Text GLabel 900  4950 0    50   Input ~ 0
LED_SDA
Text GLabel 900  4850 0    50   Input ~ 0
LED_SCL
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60BA4
P 2850 4200
AR Path="/61A6EF5C/61E60BA4" Ref="D?"  Part="1" 
AR Path="/61E60BA4" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60BA4" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BA4" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60BA4" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60BA4" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60BA4" Ref="D?"  Part="1" 
F 0 "D?" V 2650 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 3450 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 2900 4200 50  0001 C CNN
F 3 "~" H 2900 4200 50  0001 C CNN
	1    2850 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BAA
P 2750 4650
AR Path="/61A6EF5C/61E60BAA" Ref="R?"  Part="1" 
AR Path="/61E60BAA" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BAA" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BAA" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BAA" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BAA" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BAA" Ref="R?"  Part="1" 
F 0 "R?" H 2820 4696 50  0000 L CNN
F 1 "330" H 2820 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2680 4650 50  0001 C CNN
F 3 "~" H 2750 4650 50  0001 C CNN
	1    2750 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BB0
P 2950 4650
AR Path="/61A6EF5C/61E60BB0" Ref="R?"  Part="1" 
AR Path="/61E60BB0" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BB0" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BB0" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BB0" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BB0" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BB0" Ref="R?"  Part="1" 
F 0 "R?" H 3020 4696 50  0000 L CNN
F 1 "330" H 3020 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2880 4650 50  0001 C CNN
F 3 "~" H 2950 4650 50  0001 C CNN
	1    2950 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3900 2850 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60BB7
P 2850 3800
AR Path="/61A6EF5C/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60BB7" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60BB7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2850 3650 50  0001 C CNN
F 1 "+3.3V" H 2865 3973 50  0000 C CNN
F 2 "" H 2850 3800 50  0001 C CNN
F 3 "" H 2850 3800 50  0001 C CNN
	1    2850 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60BBD
P 3400 4200
AR Path="/61A6EF5C/61E60BBD" Ref="D?"  Part="1" 
AR Path="/61E60BBD" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60BBD" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BBD" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60BBD" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60BBD" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60BBD" Ref="D?"  Part="1" 
F 0 "D?" V 3200 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 4000 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 3450 4200 50  0001 C CNN
F 3 "~" H 3450 4200 50  0001 C CNN
	1    3400 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BC3
P 3300 4650
AR Path="/61A6EF5C/61E60BC3" Ref="R?"  Part="1" 
AR Path="/61E60BC3" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BC3" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BC3" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BC3" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BC3" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BC3" Ref="R?"  Part="1" 
F 0 "R?" H 3370 4696 50  0000 L CNN
F 1 "330" H 3370 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3230 4650 50  0001 C CNN
F 3 "~" H 3300 4650 50  0001 C CNN
	1    3300 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BC9
P 3500 4650
AR Path="/61A6EF5C/61E60BC9" Ref="R?"  Part="1" 
AR Path="/61E60BC9" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BC9" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BC9" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BC9" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BC9" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BC9" Ref="R?"  Part="1" 
F 0 "R?" H 3570 4696 50  0000 L CNN
F 1 "330" H 3570 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3430 4650 50  0001 C CNN
F 3 "~" H 3500 4650 50  0001 C CNN
	1    3500 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3900 3400 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60BD0
P 3400 3800
AR Path="/61A6EF5C/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60BD0" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60BD0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3400 3650 50  0001 C CNN
F 1 "+3.3V" H 3415 3973 50  0000 C CNN
F 2 "" H 3400 3800 50  0001 C CNN
F 3 "" H 3400 3800 50  0001 C CNN
	1    3400 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60BD6
P 4200 4200
AR Path="/61A6EF5C/61E60BD6" Ref="D?"  Part="1" 
AR Path="/61E60BD6" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60BD6" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BD6" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60BD6" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60BD6" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60BD6" Ref="D?"  Part="1" 
F 0 "D?" V 4000 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 4800 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 4250 4200 50  0001 C CNN
F 3 "~" H 4250 4200 50  0001 C CNN
	1    4200 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BDC
P 4100 4650
AR Path="/61A6EF5C/61E60BDC" Ref="R?"  Part="1" 
AR Path="/61E60BDC" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BDC" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BDC" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BDC" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BDC" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BDC" Ref="R?"  Part="1" 
F 0 "R?" H 4170 4696 50  0000 L CNN
F 1 "330" H 4170 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4030 4650 50  0001 C CNN
F 3 "~" H 4100 4650 50  0001 C CNN
	1    4100 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BE2
P 4300 4650
AR Path="/61A6EF5C/61E60BE2" Ref="R?"  Part="1" 
AR Path="/61E60BE2" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BE2" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BE2" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BE2" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BE2" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BE2" Ref="R?"  Part="1" 
F 0 "R?" H 4370 4696 50  0000 L CNN
F 1 "330" H 4370 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4230 4650 50  0001 C CNN
F 3 "~" H 4300 4650 50  0001 C CNN
	1    4300 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3900 4200 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60BE9
P 4200 3800
AR Path="/61A6EF5C/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60BE9" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60BE9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4200 3650 50  0001 C CNN
F 1 "+3.3V" H 4215 3973 50  0000 C CNN
F 2 "" H 4200 3800 50  0001 C CNN
F 3 "" H 4200 3800 50  0001 C CNN
	1    4200 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60BEF
P 4750 4200
AR Path="/61A6EF5C/61E60BEF" Ref="D?"  Part="1" 
AR Path="/61E60BEF" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60BEF" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BEF" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60BEF" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60BEF" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60BEF" Ref="D?"  Part="1" 
F 0 "D?" V 4550 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 5350 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 4800 4200 50  0001 C CNN
F 3 "~" H 4800 4200 50  0001 C CNN
	1    4750 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BF5
P 4650 4650
AR Path="/61A6EF5C/61E60BF5" Ref="R?"  Part="1" 
AR Path="/61E60BF5" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BF5" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BF5" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BF5" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BF5" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BF5" Ref="R?"  Part="1" 
F 0 "R?" H 4720 4696 50  0000 L CNN
F 1 "330" H 4720 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4580 4650 50  0001 C CNN
F 3 "~" H 4650 4650 50  0001 C CNN
	1    4650 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60BFB
P 4850 4650
AR Path="/61A6EF5C/61E60BFB" Ref="R?"  Part="1" 
AR Path="/61E60BFB" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60BFB" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60BFB" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60BFB" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60BFB" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60BFB" Ref="R?"  Part="1" 
F 0 "R?" H 4920 4696 50  0000 L CNN
F 1 "330" H 4920 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4780 4650 50  0001 C CNN
F 3 "~" H 4850 4650 50  0001 C CNN
	1    4850 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3900 4750 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C02
P 4750 3800
AR Path="/61A6EF5C/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C02" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C02" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4750 3650 50  0001 C CNN
F 1 "+3.3V" H 4765 3973 50  0000 C CNN
F 2 "" H 4750 3800 50  0001 C CNN
F 3 "" H 4750 3800 50  0001 C CNN
	1    4750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 4800 2750 4850
Wire Wire Line
	2750 4850 2600 4850
Wire Wire Line
	2600 4950 2950 4950
Wire Wire Line
	2950 4950 2950 4800
Wire Wire Line
	2600 5050 3300 5050
Wire Wire Line
	3300 5050 3300 4800
Wire Wire Line
	3500 4800 3500 5150
Wire Wire Line
	3500 5150 2600 5150
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60C10
P 5500 4200
AR Path="/61A6EF5C/61E60C10" Ref="D?"  Part="1" 
AR Path="/61E60C10" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60C10" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C10" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60C10" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60C10" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60C10" Ref="D?"  Part="1" 
F 0 "D?" V 5300 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 6100 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 5550 4200 50  0001 C CNN
F 3 "~" H 5550 4200 50  0001 C CNN
	1    5500 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C16
P 5400 4650
AR Path="/61A6EF5C/61E60C16" Ref="R?"  Part="1" 
AR Path="/61E60C16" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C16" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C16" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C16" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C16" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C16" Ref="R?"  Part="1" 
F 0 "R?" H 5470 4696 50  0000 L CNN
F 1 "330" H 5470 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5330 4650 50  0001 C CNN
F 3 "~" H 5400 4650 50  0001 C CNN
	1    5400 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C1C
P 5600 4650
AR Path="/61A6EF5C/61E60C1C" Ref="R?"  Part="1" 
AR Path="/61E60C1C" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C1C" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C1C" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C1C" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C1C" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C1C" Ref="R?"  Part="1" 
F 0 "R?" H 5670 4696 50  0000 L CNN
F 1 "330" H 5670 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5530 4650 50  0001 C CNN
F 3 "~" H 5600 4650 50  0001 C CNN
	1    5600 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3900 5500 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C23
P 5500 3800
AR Path="/61A6EF5C/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C23" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C23" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5500 3650 50  0001 C CNN
F 1 "+3.3V" H 5515 3973 50  0000 C CNN
F 2 "" H 5500 3800 50  0001 C CNN
F 3 "" H 5500 3800 50  0001 C CNN
	1    5500 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4800 5400 5650
Wire Wire Line
	5600 4800 5600 5750
Text GLabel 900  5150 0    50   Input ~ 0
LED_OE
Wire Wire Line
	4300 5350 2600 5350
Wire Wire Line
	2600 5250 4100 5250
$Comp
L Device:C C?
U 1 1 61E60C2E
P 1050 4150
AR Path="/5EDCCD5B/61E60C2E" Ref="C?"  Part="1" 
AR Path="/5EDCCDFD/61E60C2E" Ref="C?"  Part="1" 
AR Path="/61A6EF5C/61E60C2E" Ref="C?"  Part="1" 
AR Path="/61E60C2E" Ref="C?"  Part="1" 
AR Path="/619A998C/61E60C2E" Ref="C?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C2E" Ref="C?"  Part="1" 
AR Path="/61C541EA/61E60C2E" Ref="C?"  Part="1" 
AR Path="/61D4151D/61E60C2E" Ref="C?"  Part="1" 
AR Path="/61D60973/61E60C2E" Ref="C?"  Part="1" 
F 0 "C?" H 1165 4196 50  0000 L CNN
F 1 "10u" H 1165 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1088 4000 50  0001 C CNN
F 3 "" H 1050 4150 50  0001 C CNN
	1    1050 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61E60C34
P 1450 4150
AR Path="/5EDCCD5B/61E60C34" Ref="C?"  Part="1" 
AR Path="/5EDCCDFD/61E60C34" Ref="C?"  Part="1" 
AR Path="/61A6EF5C/61E60C34" Ref="C?"  Part="1" 
AR Path="/61E60C34" Ref="C?"  Part="1" 
AR Path="/619A998C/61E60C34" Ref="C?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C34" Ref="C?"  Part="1" 
AR Path="/61C541EA/61E60C34" Ref="C?"  Part="1" 
AR Path="/61D4151D/61E60C34" Ref="C?"  Part="1" 
AR Path="/61D60973/61E60C34" Ref="C?"  Part="1" 
F 0 "C?" H 1565 4196 50  0000 L CNN
F 1 "100n" H 1565 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1488 4000 50  0001 C CNN
F 3 "" H 1450 4150 50  0001 C CNN
	1    1450 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61E60C3A
P 1450 4300
AR Path="/61A6EF5C/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C3A" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C3A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1450 4050 50  0001 C CNN
F 1 "GND" H 1455 4127 50  0000 C CNN
F 2 "" H 1450 4300 50  0001 C CNN
F 3 "" H 1450 4300 50  0001 C CNN
	1    1450 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61E60C40
P 1050 4300
AR Path="/61A6EF5C/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C40" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C40" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1050 4050 50  0001 C CNN
F 1 "GND" H 1055 4127 50  0000 C CNN
F 2 "" H 1050 4300 50  0001 C CNN
F 3 "" H 1050 4300 50  0001 C CNN
	1    1050 4300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C46
P 1450 4000
AR Path="/61A6EF5C/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C46" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C46" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1450 3850 50  0001 C CNN
F 1 "+3.3V" H 1465 4173 50  0000 C CNN
F 2 "" H 1450 4000 50  0001 C CNN
F 3 "" H 1450 4000 50  0001 C CNN
	1    1450 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C4C
P 1050 4000
AR Path="/61A6EF5C/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C4C" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C4C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1050 3850 50  0001 C CNN
F 1 "+3.3V" H 1065 4173 50  0000 C CNN
F 2 "" H 1050 4000 50  0001 C CNN
F 3 "" H 1050 4000 50  0001 C CNN
	1    1050 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4800 4100 5250
Wire Wire Line
	2600 5450 4650 5450
Wire Wire Line
	4300 4800 4300 5350
Wire Wire Line
	2600 5550 4850 5550
Wire Wire Line
	4650 4800 4650 5450
Wire Wire Line
	2600 5650 5400 5650
Wire Wire Line
	4850 4800 4850 5550
Wire Wire Line
	2600 5750 5600 5750
Wire Wire Line
	1100 5050 1200 5050
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60C5B
P 6050 4200
AR Path="/61A6EF5C/61E60C5B" Ref="D?"  Part="1" 
AR Path="/61E60C5B" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60C5B" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C5B" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60C5B" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60C5B" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60C5B" Ref="D?"  Part="1" 
F 0 "D?" V 5850 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 6650 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 6100 4200 50  0001 C CNN
F 3 "~" H 6100 4200 50  0001 C CNN
	1    6050 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C61
P 5950 4650
AR Path="/61A6EF5C/61E60C61" Ref="R?"  Part="1" 
AR Path="/61E60C61" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C61" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C61" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C61" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C61" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C61" Ref="R?"  Part="1" 
F 0 "R?" H 6020 4696 50  0000 L CNN
F 1 "330" H 6020 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5880 4650 50  0001 C CNN
F 3 "~" H 5950 4650 50  0001 C CNN
	1    5950 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C67
P 6150 4650
AR Path="/61A6EF5C/61E60C67" Ref="R?"  Part="1" 
AR Path="/61E60C67" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C67" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C67" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C67" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C67" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C67" Ref="R?"  Part="1" 
F 0 "R?" H 6220 4696 50  0000 L CNN
F 1 "330" H 6220 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6080 4650 50  0001 C CNN
F 3 "~" H 6150 4650 50  0001 C CNN
	1    6150 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3900 6050 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C6E
P 6050 3800
AR Path="/61A6EF5C/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C6E" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C6E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6050 3650 50  0001 C CNN
F 1 "+3.3V" H 6065 3973 50  0000 C CNN
F 2 "" H 6050 3800 50  0001 C CNN
F 3 "" H 6050 3800 50  0001 C CNN
	1    6050 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 4800 5950 5850
Wire Wire Line
	6150 4800 6150 5950
Wire Wire Line
	2600 5850 5950 5850
Wire Wire Line
	2600 5950 6150 5950
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60C78
P 6600 4200
AR Path="/61A6EF5C/61E60C78" Ref="D?"  Part="1" 
AR Path="/61E60C78" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60C78" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C78" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60C78" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60C78" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60C78" Ref="D?"  Part="1" 
F 0 "D?" V 6400 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 7200 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 6650 4200 50  0001 C CNN
F 3 "~" H 6650 4200 50  0001 C CNN
	1    6600 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C7E
P 6500 4650
AR Path="/61A6EF5C/61E60C7E" Ref="R?"  Part="1" 
AR Path="/61E60C7E" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C7E" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C7E" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C7E" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C7E" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C7E" Ref="R?"  Part="1" 
F 0 "R?" H 6570 4696 50  0000 L CNN
F 1 "330" H 6570 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6430 4650 50  0001 C CNN
F 3 "~" H 6500 4650 50  0001 C CNN
	1    6500 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C84
P 6700 4650
AR Path="/61A6EF5C/61E60C84" Ref="R?"  Part="1" 
AR Path="/61E60C84" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C84" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C84" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C84" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C84" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C84" Ref="R?"  Part="1" 
F 0 "R?" H 6770 4696 50  0000 L CNN
F 1 "330" H 6770 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6630 4650 50  0001 C CNN
F 3 "~" H 6700 4650 50  0001 C CNN
	1    6700 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3900 6600 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60C8B
P 6600 3800
AR Path="/61A6EF5C/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60C8B" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60C8B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6600 3650 50  0001 C CNN
F 1 "+3.3V" H 6615 3973 50  0000 C CNN
F 2 "" H 6600 3800 50  0001 C CNN
F 3 "" H 6600 3800 50  0001 C CNN
	1    6600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4800 6500 6050
Wire Wire Line
	6700 4800 6700 6150
Wire Wire Line
	2600 6050 6500 6050
Wire Wire Line
	2600 6150 6700 6150
$Comp
L Device:LED_Dual_CAC D?
U 1 1 61E60C95
P 7150 4200
AR Path="/61A6EF5C/61E60C95" Ref="D?"  Part="1" 
AR Path="/61E60C95" Ref="D?"  Part="1" 
AR Path="/619A998C/61E60C95" Ref="D?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C95" Ref="D?"  Part="1" 
AR Path="/61C541EA/61E60C95" Ref="D?"  Part="1" 
AR Path="/61D4151D/61E60C95" Ref="D?"  Part="1" 
AR Path="/61D60973/61E60C95" Ref="D?"  Part="1" 
F 0 "D?" V 6950 4350 50  0000 L CNN
F 1 "LED_RED_GREEN_CA" H 7750 4200 50  0000 L CNN
F 2 "LED_THT:LED_D3.0mm-3" H 7200 4200 50  0001 C CNN
F 3 "~" H 7200 4200 50  0001 C CNN
	1    7150 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 61E60C9B
P 7050 4650
AR Path="/61A6EF5C/61E60C9B" Ref="R?"  Part="1" 
AR Path="/61E60C9B" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60C9B" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60C9B" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60C9B" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60C9B" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60C9B" Ref="R?"  Part="1" 
F 0 "R?" H 7120 4696 50  0000 L CNN
F 1 "330" H 7120 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6980 4650 50  0001 C CNN
F 3 "~" H 7050 4650 50  0001 C CNN
	1    7050 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E60CA1
P 7250 4650
AR Path="/61A6EF5C/61E60CA1" Ref="R?"  Part="1" 
AR Path="/61E60CA1" Ref="R?"  Part="1" 
AR Path="/619A998C/61E60CA1" Ref="R?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60CA1" Ref="R?"  Part="1" 
AR Path="/61C541EA/61E60CA1" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E60CA1" Ref="R?"  Part="1" 
AR Path="/61D60973/61E60CA1" Ref="R?"  Part="1" 
F 0 "R?" H 7320 4696 50  0000 L CNN
F 1 "330" H 7320 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7180 4650 50  0001 C CNN
F 3 "~" H 7250 4650 50  0001 C CNN
	1    7250 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3900 7150 3800
$Comp
L power:+3.3V #PWR?
U 1 1 61E60CA8
P 7150 3800
AR Path="/61A6EF5C/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E60CA8" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E60CA8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7150 3650 50  0001 C CNN
F 1 "+3.3V" H 7165 3973 50  0000 C CNN
F 2 "" H 7150 3800 50  0001 C CNN
F 3 "" H 7150 3800 50  0001 C CNN
	1    7150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4800 7050 6250
Wire Wire Line
	7250 4800 7250 6350
Wire Wire Line
	2600 6250 7050 6250
Wire Wire Line
	2600 6350 7250 6350
Wire Wire Line
	900  5150 1200 5150
Wire Wire Line
	900  4850 1200 4850
Wire Wire Line
	900  4950 1200 4950
Wire Wire Line
	1100 5050 1100 5850
Wire Wire Line
	900  5750 1200 5750
$Comp
L power:+3.3V #PWR?
U 1 1 61E65B46
P 900 5650
AR Path="/61A6EF5C/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A0FB81/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/61C541EA/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E65B46" Ref="#PWR?"  Part="1" 
AR Path="/61D60973/61E65B46" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 900 5500 50  0001 C CNN
F 1 "+3.3V" H 915 5823 50  0000 C CNN
F 2 "" H 900 5650 50  0001 C CNN
F 3 "" H 900 5650 50  0001 C CNN
	1    900  5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  5650 900  5750
Wire Wire Line
	1200 2800 1100 2800
Connection ~ 1100 2800
Wire Wire Line
	1100 2800 1100 2900
$Comp
L Switch:SW_SPST SW?
U 1 1 61E99C41
P 10500 1900
AR Path="/61A6EF5C/61E99C41" Ref="SW?"  Part="1" 
AR Path="/61E99C41" Ref="SW?"  Part="1" 
AR Path="/619A998C/61E99C41" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C41" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61E99C41" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61E99C41" Ref="SW?"  Part="1" 
AR Path="/61D60973/61E99C41" Ref="SW?"  Part="1" 
F 0 "SW?" H 10500 2135 50  0000 C CNN
F 1 "BTTN_PLAY_A" H 10500 2044 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10500 1900 50  0001 C CNN
F 3 "~" H 10500 1900 50  0001 C CNN
	1    10500 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E99C47
P 10050 1650
AR Path="/61A6EF5C/61E99C47" Ref="R?"  Part="1" 
AR Path="/61E99C47" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99C47" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C47" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99C47" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99C47" Ref="R?"  Part="1" 
F 0 "R?" H 10120 1696 50  0000 L CNN
F 1 "10k" H 10120 1605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9980 1650 50  0001 C CNN
F 3 "~" H 10050 1650 50  0001 C CNN
	1    10050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 1800 10050 1900
Wire Wire Line
	10050 1900 10300 1900
$Comp
L power:+3.3V #PWR?
U 1 1 61E99C4F
P 10050 1400
AR Path="/61A6EF5C/61E99C4F" Ref="#PWR?"  Part="1" 
AR Path="/61E99C4F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C4F" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C4F" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C4F" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C4F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10050 1250 50  0001 C CNN
F 1 "+3.3V" H 10065 1573 50  0000 C CNN
F 2 "" H 10050 1400 50  0001 C CNN
F 3 "" H 10050 1400 50  0001 C CNN
	1    10050 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 1400 10050 1500
Wire Wire Line
	10050 1900 9950 1900
Connection ~ 10050 1900
Wire Wire Line
	10700 1900 10800 1900
Wire Wire Line
	10800 1900 10800 2000
$Comp
L power:GND #PWR?
U 1 1 61E99C5A
P 10800 2000
AR Path="/61A6EF5C/61E99C5A" Ref="#PWR?"  Part="1" 
AR Path="/61E99C5A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C5A" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C5A" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C5A" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C5A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10800 1750 50  0001 C CNN
F 1 "GND" H 10805 1827 50  0000 C CNN
F 2 "" H 10800 2000 50  0001 C CNN
F 3 "" H 10800 2000 50  0001 C CNN
	1    10800 2000
	1    0    0    -1  
$EndComp
Text GLabel 9950 1900 0    50   Output ~ 0
BTTN_PLAY_A
$Comp
L Device:R R?
U 1 1 61E99C61
P 8250 2800
AR Path="/61A6EF5C/61E99C61" Ref="R?"  Part="1" 
AR Path="/61E99C61" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99C61" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C61" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99C61" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99C61" Ref="R?"  Part="1" 
F 0 "R?" H 8000 2850 50  0000 L CNN
F 1 "10k" H 8000 2750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8180 2800 50  0001 C CNN
F 3 "~" H 8250 2800 50  0001 C CNN
	1    8250 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2950 8250 3050
$Comp
L power:+3.3V #PWR?
U 1 1 61E99C68
P 8250 2550
AR Path="/61A6EF5C/61E99C68" Ref="#PWR?"  Part="1" 
AR Path="/61E99C68" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C68" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C68" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C68" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C68" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8250 2400 50  0001 C CNN
F 1 "+3.3V" H 8265 2723 50  0000 C CNN
F 2 "" H 8250 2550 50  0001 C CNN
F 3 "" H 8250 2550 50  0001 C CNN
	1    8250 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2550 8250 2650
Wire Wire Line
	8250 3050 8150 3050
Connection ~ 8250 3050
Text GLabel 8150 3050 0    50   Output ~ 0
BTTN_UP_B
$Comp
L Device:R R?
U 1 1 61E99C72
P 8550 2800
AR Path="/61A6EF5C/61E99C72" Ref="R?"  Part="1" 
AR Path="/61E99C72" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99C72" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C72" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99C72" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99C72" Ref="R?"  Part="1" 
F 0 "R?" H 8620 2846 50  0000 L CNN
F 1 "10k" H 8620 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8480 2800 50  0001 C CNN
F 3 "~" H 8550 2800 50  0001 C CNN
	1    8550 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61E99C78
P 8550 2550
AR Path="/61A6EF5C/61E99C78" Ref="#PWR?"  Part="1" 
AR Path="/61E99C78" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C78" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C78" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C78" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C78" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 2400 50  0001 C CNN
F 1 "+3.3V" H 8565 2723 50  0000 C CNN
F 2 "" H 8550 2550 50  0001 C CNN
F 3 "" H 8550 2550 50  0001 C CNN
	1    8550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 2550 8550 2650
Text GLabel 8150 3250 0    50   Output ~ 0
BTTN_DOWN_B
$Comp
L Switch:SW_SPST SW?
U 1 1 61E99C80
P 10500 3150
AR Path="/61A6EF5C/61E99C80" Ref="SW?"  Part="1" 
AR Path="/61E99C80" Ref="SW?"  Part="1" 
AR Path="/619A998C/61E99C80" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C80" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61E99C80" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61E99C80" Ref="SW?"  Part="1" 
AR Path="/61D60973/61E99C80" Ref="SW?"  Part="1" 
F 0 "SW?" H 10500 3385 50  0000 C CNN
F 1 "BTTN_PLAY_B" H 10500 3294 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10500 3150 50  0001 C CNN
F 3 "~" H 10500 3150 50  0001 C CNN
	1    10500 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E99C86
P 10050 2900
AR Path="/61A6EF5C/61E99C86" Ref="R?"  Part="1" 
AR Path="/61E99C86" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99C86" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C86" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99C86" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99C86" Ref="R?"  Part="1" 
F 0 "R?" H 10120 2946 50  0000 L CNN
F 1 "10k" H 10120 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9980 2900 50  0001 C CNN
F 3 "~" H 10050 2900 50  0001 C CNN
	1    10050 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 3050 10050 3150
Wire Wire Line
	10050 3150 10300 3150
$Comp
L power:+3.3V #PWR?
U 1 1 61E99C8E
P 10050 2650
AR Path="/61A6EF5C/61E99C8E" Ref="#PWR?"  Part="1" 
AR Path="/61E99C8E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C8E" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C8E" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C8E" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C8E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10050 2500 50  0001 C CNN
F 1 "+3.3V" H 10065 2823 50  0000 C CNN
F 2 "" H 10050 2650 50  0001 C CNN
F 3 "" H 10050 2650 50  0001 C CNN
	1    10050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 2650 10050 2750
Wire Wire Line
	10050 3150 9950 3150
Connection ~ 10050 3150
Wire Wire Line
	10700 3150 10800 3150
Wire Wire Line
	10800 3150 10800 3250
$Comp
L power:GND #PWR?
U 1 1 61E99C99
P 10800 3250
AR Path="/61A6EF5C/61E99C99" Ref="#PWR?"  Part="1" 
AR Path="/61E99C99" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99C99" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99C99" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99C99" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99C99" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10800 3000 50  0001 C CNN
F 1 "GND" H 10805 3077 50  0000 C CNN
F 2 "" H 10800 3250 50  0001 C CNN
F 3 "" H 10800 3250 50  0001 C CNN
	1    10800 3250
	1    0    0    -1  
$EndComp
Text GLabel 9950 3150 0    50   Output ~ 0
BTTN_PLAY_B
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61E99CA0
P 8850 3150
AR Path="/61D4151D/61E99CA0" Ref="SW?"  Part="1" 
AR Path="/61D60973/61E99CA0" Ref="SW?"  Part="1" 
F 0 "SW?" H 8850 3500 50  0000 C CNN
F 1 "BTTN_SPEED_B" H 9000 3400 50  0000 C CNN
F 2 "" H 8850 3150 50  0001 C CNN
F 3 "~" H 8850 3150 50  0001 C CNN
	1    8850 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	8250 3050 8650 3050
Wire Wire Line
	8650 3250 8550 3250
Wire Wire Line
	8550 3250 8550 2950
Wire Wire Line
	8150 3250 8550 3250
Connection ~ 8550 3250
Wire Wire Line
	9050 3150 9100 3150
Wire Wire Line
	9100 3150 9100 3250
$Comp
L power:GND #PWR?
U 1 1 61E99CAD
P 9100 3250
AR Path="/61A6EF5C/61E99CAD" Ref="#PWR?"  Part="1" 
AR Path="/61E99CAD" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99CAD" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CAD" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99CAD" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99CAD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9100 3000 50  0001 C CNN
F 1 "GND" H 9105 3077 50  0000 C CNN
F 2 "" H 9100 3250 50  0001 C CNN
F 3 "" H 9100 3250 50  0001 C CNN
	1    9100 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61E99CB3
P 8250 1550
AR Path="/61A6EF5C/61E99CB3" Ref="R?"  Part="1" 
AR Path="/61E99CB3" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99CB3" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CB3" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99CB3" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99CB3" Ref="R?"  Part="1" 
F 0 "R?" H 8000 1600 50  0000 L CNN
F 1 "10k" H 8000 1500 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8180 1550 50  0001 C CNN
F 3 "~" H 8250 1550 50  0001 C CNN
	1    8250 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1700 8250 1800
$Comp
L power:+3.3V #PWR?
U 1 1 61E99CBA
P 8250 1300
AR Path="/61A6EF5C/61E99CBA" Ref="#PWR?"  Part="1" 
AR Path="/61E99CBA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99CBA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CBA" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99CBA" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99CBA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8250 1150 50  0001 C CNN
F 1 "+3.3V" H 8265 1473 50  0000 C CNN
F 2 "" H 8250 1300 50  0001 C CNN
F 3 "" H 8250 1300 50  0001 C CNN
	1    8250 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1300 8250 1400
Wire Wire Line
	8250 1800 8150 1800
Connection ~ 8250 1800
Text GLabel 8150 1800 0    50   Output ~ 0
BTTN_UP_A
$Comp
L Device:R R?
U 1 1 61E99CC4
P 8550 1550
AR Path="/61A6EF5C/61E99CC4" Ref="R?"  Part="1" 
AR Path="/61E99CC4" Ref="R?"  Part="1" 
AR Path="/619A998C/61E99CC4" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CC4" Ref="R?"  Part="1" 
AR Path="/61C541EC/61E99CC4" Ref="R?"  Part="1" 
AR Path="/61D4151D/61E99CC4" Ref="R?"  Part="1" 
F 0 "R?" H 8620 1596 50  0000 L CNN
F 1 "10k" H 8620 1505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8480 1550 50  0001 C CNN
F 3 "~" H 8550 1550 50  0001 C CNN
	1    8550 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61E99CCA
P 8550 1300
AR Path="/61A6EF5C/61E99CCA" Ref="#PWR?"  Part="1" 
AR Path="/61E99CCA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99CCA" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CCA" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99CCA" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99CCA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 1150 50  0001 C CNN
F 1 "+3.3V" H 8565 1473 50  0000 C CNN
F 2 "" H 8550 1300 50  0001 C CNN
F 3 "" H 8550 1300 50  0001 C CNN
	1    8550 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 1300 8550 1400
Text GLabel 8150 2000 0    50   Output ~ 0
BTTN_DOWN_A
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61E99CD2
P 8850 1900
F 0 "SW?" H 8850 2250 50  0000 C CNN
F 1 "BTTN_SPEED_A" H 9000 2150 50  0000 C CNN
F 2 "" H 8850 1900 50  0001 C CNN
F 3 "~" H 8850 1900 50  0001 C CNN
	1    8850 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	8250 1800 8650 1800
Wire Wire Line
	8650 2000 8550 2000
Wire Wire Line
	8550 2000 8550 1700
Wire Wire Line
	8150 2000 8550 2000
Connection ~ 8550 2000
Wire Wire Line
	9050 1900 9100 1900
Wire Wire Line
	9100 1900 9100 2000
$Comp
L power:GND #PWR?
U 1 1 61E99CDF
P 9100 2000
AR Path="/61A6EF5C/61E99CDF" Ref="#PWR?"  Part="1" 
AR Path="/61E99CDF" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61E99CDF" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61E99CDF" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61E99CDF" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61E99CDF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9100 1750 50  0001 C CNN
F 1 "GND" H 9105 1827 50  0000 C CNN
F 2 "" H 9100 2000 50  0001 C CNN
F 3 "" H 9100 2000 50  0001 C CNN
	1    9100 2000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW?
U 1 1 61EFBD79
P 10500 4400
AR Path="/61A6EF5C/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/619A998C/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61EFBD79" Ref="SW?"  Part="1" 
AR Path="/61D60973/61EFBD79" Ref="SW?"  Part="1" 
F 0 "SW?" H 10500 4635 50  0000 C CNN
F 1 "BTTN_PLAY_C" H 10500 4544 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10500 4400 50  0001 C CNN
F 3 "~" H 10500 4400 50  0001 C CNN
	1    10500 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61EFBD7F
P 10050 4150
AR Path="/61A6EF5C/61EFBD7F" Ref="R?"  Part="1" 
AR Path="/61EFBD7F" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBD7F" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBD7F" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBD7F" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBD7F" Ref="R?"  Part="1" 
F 0 "R?" H 10120 4196 50  0000 L CNN
F 1 "10k" H 10120 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9980 4150 50  0001 C CNN
F 3 "~" H 10050 4150 50  0001 C CNN
	1    10050 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 4300 10050 4400
Wire Wire Line
	10050 4400 10300 4400
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBD87
P 10050 3900
AR Path="/61A6EF5C/61EFBD87" Ref="#PWR?"  Part="1" 
AR Path="/61EFBD87" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBD87" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBD87" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBD87" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBD87" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10050 3750 50  0001 C CNN
F 1 "+3.3V" H 10065 4073 50  0000 C CNN
F 2 "" H 10050 3900 50  0001 C CNN
F 3 "" H 10050 3900 50  0001 C CNN
	1    10050 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 3900 10050 4000
Wire Wire Line
	10050 4400 9950 4400
Connection ~ 10050 4400
Wire Wire Line
	10700 4400 10800 4400
Wire Wire Line
	10800 4400 10800 4500
$Comp
L power:GND #PWR?
U 1 1 61EFBD92
P 10800 4500
AR Path="/61A6EF5C/61EFBD92" Ref="#PWR?"  Part="1" 
AR Path="/61EFBD92" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBD92" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBD92" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBD92" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBD92" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10800 4250 50  0001 C CNN
F 1 "GND" H 10805 4327 50  0000 C CNN
F 2 "" H 10800 4500 50  0001 C CNN
F 3 "" H 10800 4500 50  0001 C CNN
	1    10800 4500
	1    0    0    -1  
$EndComp
Text GLabel 9950 4400 0    50   Output ~ 0
BTTN_PLAY_C
$Comp
L Device:R R?
U 1 1 61EFBD99
P 8250 5300
AR Path="/61A6EF5C/61EFBD99" Ref="R?"  Part="1" 
AR Path="/61EFBD99" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBD99" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBD99" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBD99" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBD99" Ref="R?"  Part="1" 
F 0 "R?" H 8000 5350 50  0000 L CNN
F 1 "10k" H 8000 5250 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8180 5300 50  0001 C CNN
F 3 "~" H 8250 5300 50  0001 C CNN
	1    8250 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 5450 8250 5550
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBDA0
P 8250 5050
AR Path="/61A6EF5C/61EFBDA0" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDA0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDA0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDA0" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDA0" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDA0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8250 4900 50  0001 C CNN
F 1 "+3.3V" H 8265 5223 50  0000 C CNN
F 2 "" H 8250 5050 50  0001 C CNN
F 3 "" H 8250 5050 50  0001 C CNN
	1    8250 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 5050 8250 5150
Wire Wire Line
	8250 5550 8150 5550
Connection ~ 8250 5550
Text GLabel 8150 5550 0    50   Output ~ 0
BTTN_UP_D
$Comp
L Device:R R?
U 1 1 61EFBDAA
P 8550 5300
AR Path="/61A6EF5C/61EFBDAA" Ref="R?"  Part="1" 
AR Path="/61EFBDAA" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBDAA" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDAA" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBDAA" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBDAA" Ref="R?"  Part="1" 
F 0 "R?" H 8620 5346 50  0000 L CNN
F 1 "10k" H 8620 5255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8480 5300 50  0001 C CNN
F 3 "~" H 8550 5300 50  0001 C CNN
	1    8550 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBDB0
P 8550 5050
AR Path="/61A6EF5C/61EFBDB0" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDB0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDB0" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDB0" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDB0" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDB0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 4900 50  0001 C CNN
F 1 "+3.3V" H 8565 5223 50  0000 C CNN
F 2 "" H 8550 5050 50  0001 C CNN
F 3 "" H 8550 5050 50  0001 C CNN
	1    8550 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 5050 8550 5150
Text GLabel 8150 5750 0    50   Output ~ 0
BTTN_DOWN_D
$Comp
L Switch:SW_SPST SW?
U 1 1 61EFBDB8
P 10500 5650
AR Path="/61A6EF5C/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/619A998C/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/61C541EC/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/61D4151D/61EFBDB8" Ref="SW?"  Part="1" 
AR Path="/61D60973/61EFBDB8" Ref="SW?"  Part="1" 
F 0 "SW?" H 10500 5885 50  0000 C CNN
F 1 "BTTN_PLAY_D" H 10500 5794 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10500 5650 50  0001 C CNN
F 3 "~" H 10500 5650 50  0001 C CNN
	1    10500 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61EFBDBE
P 10050 5400
AR Path="/61A6EF5C/61EFBDBE" Ref="R?"  Part="1" 
AR Path="/61EFBDBE" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBDBE" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDBE" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBDBE" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBDBE" Ref="R?"  Part="1" 
F 0 "R?" H 10120 5446 50  0000 L CNN
F 1 "10k" H 10120 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9980 5400 50  0001 C CNN
F 3 "~" H 10050 5400 50  0001 C CNN
	1    10050 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5550 10050 5650
Wire Wire Line
	10050 5650 10300 5650
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBDC6
P 10050 5150
AR Path="/61A6EF5C/61EFBDC6" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDC6" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDC6" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDC6" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDC6" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDC6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10050 5000 50  0001 C CNN
F 1 "+3.3V" H 10065 5323 50  0000 C CNN
F 2 "" H 10050 5150 50  0001 C CNN
F 3 "" H 10050 5150 50  0001 C CNN
	1    10050 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5150 10050 5250
Wire Wire Line
	10050 5650 9950 5650
Connection ~ 10050 5650
Wire Wire Line
	10700 5650 10800 5650
Wire Wire Line
	10800 5650 10800 5750
$Comp
L power:GND #PWR?
U 1 1 61EFBDD1
P 10800 5750
AR Path="/61A6EF5C/61EFBDD1" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDD1" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDD1" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDD1" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDD1" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDD1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10800 5500 50  0001 C CNN
F 1 "GND" H 10805 5577 50  0000 C CNN
F 2 "" H 10800 5750 50  0001 C CNN
F 3 "" H 10800 5750 50  0001 C CNN
	1    10800 5750
	1    0    0    -1  
$EndComp
Text GLabel 9950 5650 0    50   Output ~ 0
BTTN_PLAY_D
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61EFBDD8
P 8850 5650
AR Path="/61D4151D/61EFBDD8" Ref="SW?"  Part="1" 
AR Path="/61D60973/61EFBDD8" Ref="SW?"  Part="1" 
F 0 "SW?" H 8850 6000 50  0000 C CNN
F 1 "BTTN_SPEED_D" H 9000 5900 50  0000 C CNN
F 2 "" H 8850 5650 50  0001 C CNN
F 3 "~" H 8850 5650 50  0001 C CNN
	1    8850 5650
	-1   0    0    1   
$EndComp
Wire Wire Line
	8250 5550 8650 5550
Wire Wire Line
	8650 5750 8550 5750
Wire Wire Line
	8550 5750 8550 5450
Wire Wire Line
	8150 5750 8550 5750
Connection ~ 8550 5750
Wire Wire Line
	9050 5650 9100 5650
Wire Wire Line
	9100 5650 9100 5750
$Comp
L power:GND #PWR?
U 1 1 61EFBDE5
P 9100 5750
AR Path="/61A6EF5C/61EFBDE5" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDE5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDE5" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDE5" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDE5" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDE5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9100 5500 50  0001 C CNN
F 1 "GND" H 9105 5577 50  0000 C CNN
F 2 "" H 9100 5750 50  0001 C CNN
F 3 "" H 9100 5750 50  0001 C CNN
	1    9100 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61EFBDEB
P 8250 4050
AR Path="/61A6EF5C/61EFBDEB" Ref="R?"  Part="1" 
AR Path="/61EFBDEB" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBDEB" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDEB" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBDEB" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBDEB" Ref="R?"  Part="1" 
F 0 "R?" H 8000 4100 50  0000 L CNN
F 1 "10k" H 8000 4000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8180 4050 50  0001 C CNN
F 3 "~" H 8250 4050 50  0001 C CNN
	1    8250 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4200 8250 4300
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBDF2
P 8250 3800
AR Path="/61A6EF5C/61EFBDF2" Ref="#PWR?"  Part="1" 
AR Path="/61EFBDF2" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBDF2" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDF2" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBDF2" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBDF2" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8250 3650 50  0001 C CNN
F 1 "+3.3V" H 8265 3973 50  0000 C CNN
F 2 "" H 8250 3800 50  0001 C CNN
F 3 "" H 8250 3800 50  0001 C CNN
	1    8250 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 3800 8250 3900
Wire Wire Line
	8250 4300 8150 4300
Connection ~ 8250 4300
Text GLabel 8150 4300 0    50   Output ~ 0
BTTN_UP_C
$Comp
L Device:R R?
U 1 1 61EFBDFC
P 8550 4050
AR Path="/61A6EF5C/61EFBDFC" Ref="R?"  Part="1" 
AR Path="/61EFBDFC" Ref="R?"  Part="1" 
AR Path="/619A998C/61EFBDFC" Ref="R?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBDFC" Ref="R?"  Part="1" 
AR Path="/61C541EC/61EFBDFC" Ref="R?"  Part="1" 
AR Path="/61D4151D/61EFBDFC" Ref="R?"  Part="1" 
F 0 "R?" H 8620 4096 50  0000 L CNN
F 1 "10k" H 8620 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8480 4050 50  0001 C CNN
F 3 "~" H 8550 4050 50  0001 C CNN
	1    8550 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61EFBE02
P 8550 3800
AR Path="/61A6EF5C/61EFBE02" Ref="#PWR?"  Part="1" 
AR Path="/61EFBE02" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBE02" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBE02" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBE02" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBE02" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 3650 50  0001 C CNN
F 1 "+3.3V" H 8565 3973 50  0000 C CNN
F 2 "" H 8550 3800 50  0001 C CNN
F 3 "" H 8550 3800 50  0001 C CNN
	1    8550 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3800 8550 3900
Text GLabel 8150 4500 0    50   Output ~ 0
BTTN_DOWN_C
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 61EFBE0A
P 8850 4400
AR Path="/61D4151D/61EFBE0A" Ref="SW?"  Part="1" 
AR Path="/61D60973/61EFBE0A" Ref="SW?"  Part="1" 
F 0 "SW?" H 8850 4750 50  0000 C CNN
F 1 "BTTN_SPEED_C" H 9000 4650 50  0000 C CNN
F 2 "" H 8850 4400 50  0001 C CNN
F 3 "~" H 8850 4400 50  0001 C CNN
	1    8850 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	8250 4300 8650 4300
Wire Wire Line
	8650 4500 8550 4500
Wire Wire Line
	8550 4500 8550 4200
Wire Wire Line
	8150 4500 8550 4500
Connection ~ 8550 4500
Wire Wire Line
	9050 4400 9100 4400
Wire Wire Line
	9100 4400 9100 4500
$Comp
L power:GND #PWR?
U 1 1 61EFBE17
P 9100 4500
AR Path="/61A6EF5C/61EFBE17" Ref="#PWR?"  Part="1" 
AR Path="/61EFBE17" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61EFBE17" Ref="#PWR?"  Part="1" 
AR Path="/619A998C/61A2476B/61EFBE17" Ref="#PWR?"  Part="1" 
AR Path="/61C541EC/61EFBE17" Ref="#PWR?"  Part="1" 
AR Path="/61D4151D/61EFBE17" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9100 4250 50  0001 C CNN
F 1 "GND" H 9105 4327 50  0000 C CNN
F 2 "" H 9100 4500 50  0001 C CNN
F 3 "" H 9100 4500 50  0001 C CNN
	1    9100 4500
	1    0    0    -1  
$EndComp
$EndSCHEMATC

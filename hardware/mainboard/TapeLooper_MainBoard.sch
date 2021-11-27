EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1050 6250 700  500 
U 61C2BD72
F0 "Power" 50
F1 "Power.sch" 50
$EndSheet
$Sheet
S 1050 5250 700  500 
U 61C2C7F7
F0 "AudioIO" 50
F1 "AudioIO.sch" 50
$EndSheet
$Comp
L power:GNDA #PWR?
U 1 1 61CE6E8E
P 6400 3200
AR Path="/61CDF9A8/61CE6E8E" Ref="#PWR?"  Part="1" 
AR Path="/61CE6E8E" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 6400 2950 50  0001 C CNN
F 1 "GNDA" H 6405 3027 50  0000 C CNN
F 2 "" H 6400 3200 50  0001 C CNN
F 3 "" H 6400 3200 50  0001 C CNN
	1    6400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3200 6400 3100
Wire Wire Line
	6400 3100 7550 3100
Text GLabel 7450 2700 0    50   Input ~ 0
AUDIO_ADC_L
Text GLabel 7450 2800 0    50   Input ~ 0
AUDIO_ADC_R
Text GLabel 7450 2900 0    50   Output ~ 0
AUDIO_DAC_L
Text GLabel 7450 3000 0    50   Output ~ 0
AUDIO_DAC_R
Wire Wire Line
	7450 3000 7550 3000
Wire Wire Line
	7450 2900 7550 2900
Wire Wire Line
	7450 2800 7550 2800
Wire Wire Line
	7450 2700 7550 2700
$Comp
L ell_special:DAISY_SEED U?
U 1 1 61CE6E9E
P 9100 3150
AR Path="/61CDF9A8/61CE6E9E" Ref="U?"  Part="1" 
AR Path="/61CE6E9E" Ref="U1"  Part="1" 
F 0 "U1" H 10550 5250 50  0000 C CNN
F 1 "DAISY_SEED" H 7850 5250 50  0000 C CNN
F 2 "ell_special:Daisy_Seed" H 6850 4250 50  0001 C CNN
F 3 "" H 6850 4250 50  0001 C CNN
	1    9100 3150
	1    0    0    -1  
$EndComp
Text Notes 6350 5250 0    50   ~ 0
external Vreg for 3V3D,\ninternal Vreg too small\n
Wire Wire Line
	7550 5000 7150 5000
$Comp
L power:GND #PWR?
U 1 1 61CE6EB1
P 7450 5200
AR Path="/61CDF9A8/61CE6EB1" Ref="#PWR?"  Part="1" 
AR Path="/61CE6EB1" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 7450 4950 50  0001 C CNN
F 1 "GND" H 7455 5027 50  0000 C CNN
F 2 "" H 7450 5200 50  0001 C CNN
F 3 "" H 7450 5200 50  0001 C CNN
	1    7450 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 5100 7450 5100
Wire Wire Line
	7450 5100 7450 5200
Text GLabel 7150 5000 0    50   Input ~ 0
Vin+
Text GLabel 7450 1900 0    50   Input ~ 0
BTTN_LOAD
Text GLabel 7450 2000 0    50   Input ~ 0
BTTN_SAVE
Text GLabel 7450 2600 0    50   Input ~ 0
BTTN_DIRECTION
Text GLabel 7450 1200 0    50   Input ~ 0
BTTN_REC
Wire Wire Line
	7450 2300 7550 2300
Wire Wire Line
	7450 2400 7550 2400
Text GLabel 7450 2400 0    50   Output ~ 0
LED_SDA
Text GLabel 7450 2300 0    50   Output ~ 0
LED_SCL
Wire Wire Line
	7550 2500 7450 2500
Text GLabel 7450 2500 0    50   Output ~ 0
LED_OE
$Sheet
S 1050 1150 700  500 
U 61A6EF5C
F0 "Common LEDs And Buttons" 50
F1 "Common_LEDs_And_Buttons.sch" 50
$EndSheet
$Comp
L Connector:Micro_SD_Card J1
U 1 1 61CEE073
P 5250 1600
F 0 "J1" H 5200 2317 50  0000 C CNN
F 1 "Micro_SD_Card" H 5200 2226 50  0000 C CNN
F 2 "ell_connectors:PJS008U-3000" H 6400 1900 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 5250 1600 50  0001 C CNN
	1    5250 1600
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61CF2D7B
P 4400 2250
AR Path="/61CDF9A8/61CF2D7B" Ref="#PWR?"  Part="1" 
AR Path="/61CF2D7B" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 4400 2000 50  0001 C CNN
F 1 "GND" H 4405 2077 50  0000 C CNN
F 2 "" H 4400 2250 50  0001 C CNN
F 3 "" H 4400 2250 50  0001 C CNN
	1    4400 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2200 4400 2200
Wire Wire Line
	4400 2200 4400 2250
$Comp
L power:GND #PWR?
U 1 1 61CF352B
P 6250 2150
AR Path="/61CDF9A8/61CF352B" Ref="#PWR?"  Part="1" 
AR Path="/61CF352B" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 6250 1900 50  0001 C CNN
F 1 "GND" H 6255 1977 50  0000 C CNN
F 2 "" H 6250 2150 50  0001 C CNN
F 3 "" H 6250 2150 50  0001 C CNN
	1    6250 2150
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61CF3ADA
P 6250 1100
AR Path="/61CDF9A8/61CF3ADA" Ref="#PWR?"  Part="1" 
AR Path="/61CF3ADA" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 6250 950 50  0001 C CNN
F 1 "+3.3V" H 6265 1273 50  0000 C CNN
F 2 "" H 6250 1100 50  0001 C CNN
F 3 "" H 6250 1100 50  0001 C CNN
	1    6250 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 1600 6250 1600
Wire Wire Line
	6250 1600 6250 1100
Wire Wire Line
	6150 1800 6250 1800
Wire Wire Line
	6250 1800 6250 2150
Wire Wire Line
	6150 1900 6700 1900
Wire Wire Line
	6150 1300 7000 1300
Wire Wire Line
	7000 1300 7100 1400
Wire Wire Line
	7100 1400 7550 1400
Wire Wire Line
	6150 1400 7000 1400
Wire Wire Line
	7000 1400 7100 1300
Wire Wire Line
	7100 1300 7550 1300
Wire Wire Line
	6600 1500 7550 1500
Wire Wire Line
	6600 2000 6600 1500
Wire Wire Line
	6150 2000 6600 2000
Wire Wire Line
	6700 1600 7550 1600
Wire Wire Line
	6700 1900 6700 1600
Wire Wire Line
	7550 1700 6450 1700
Wire Wire Line
	6450 1700 6450 1500
Wire Wire Line
	6450 1500 6150 1500
Wire Wire Line
	7550 1800 6350 1800
Wire Wire Line
	6350 1800 6350 1700
Wire Wire Line
	6350 1700 6150 1700
$Comp
L Device:C C?
U 1 1 61D02D24
P 5100 2650
AR Path="/619B033C/61D02D24" Ref="C?"  Part="1" 
AR Path="/61D02D24" Ref="C1"  Part="1" 
AR Path="/61C2BD72/61D02D24" Ref="C?"  Part="1" 
F 0 "C1" H 5215 2696 50  0000 L CNN
F 1 "10u" H 5215 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5138 2500 50  0001 C CNN
F 3 "" H 5100 2650 50  0001 C CNN
	1    5100 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61D02D2A
P 5500 2650
AR Path="/619B033C/61D02D2A" Ref="C?"  Part="1" 
AR Path="/61D02D2A" Ref="C2"  Part="1" 
AR Path="/61C2BD72/61D02D2A" Ref="C?"  Part="1" 
F 0 "C2" H 5615 2696 50  0000 L CNN
F 1 "100n" H 5615 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5538 2500 50  0001 C CNN
F 3 "" H 5500 2650 50  0001 C CNN
	1    5500 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61D03900
P 5100 2500
AR Path="/61CDF9A8/61D03900" Ref="#PWR?"  Part="1" 
AR Path="/61D03900" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 5100 2350 50  0001 C CNN
F 1 "+3.3V" H 5115 2673 50  0000 C CNN
F 2 "" H 5100 2500 50  0001 C CNN
F 3 "" H 5100 2500 50  0001 C CNN
	1    5100 2500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61D044B9
P 5500 2500
AR Path="/61CDF9A8/61D044B9" Ref="#PWR?"  Part="1" 
AR Path="/61D044B9" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 5500 2350 50  0001 C CNN
F 1 "+3.3V" H 5515 2673 50  0000 C CNN
F 2 "" H 5500 2500 50  0001 C CNN
F 3 "" H 5500 2500 50  0001 C CNN
	1    5500 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D04F0C
P 5100 2800
AR Path="/61CDF9A8/61D04F0C" Ref="#PWR?"  Part="1" 
AR Path="/61D04F0C" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 5100 2550 50  0001 C CNN
F 1 "GND" H 5105 2627 50  0000 C CNN
F 2 "" H 5100 2800 50  0001 C CNN
F 3 "" H 5100 2800 50  0001 C CNN
	1    5100 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61D05807
P 5500 2800
AR Path="/61CDF9A8/61D05807" Ref="#PWR?"  Part="1" 
AR Path="/61D05807" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 5500 2550 50  0001 C CNN
F 1 "GND" H 5505 2627 50  0000 C CNN
F 2 "" H 5500 2800 50  0001 C CNN
F 3 "" H 5500 2800 50  0001 C CNN
	1    5500 2800
	1    0    0    -1  
$EndComp
Text GLabel 6750 3800 0    50   Output ~ 0
SPI_CS_AB
Text GLabel 6750 3700 0    50   Input ~ 0
SPI_MISO
Text GLabel 6750 3900 0    50   Output ~ 0
SPI_CS_CD
Text GLabel 6750 4000 0    50   Output ~ 0
SPI_SCLK
Text GLabel 6750 3600 0    50   Output ~ 0
SPI_MOSI
$Comp
L Device:R R2
U 1 1 61C9E107
P 7300 3600
F 0 "R2" V 7093 3600 50  0000 C CNN
F 1 "20R" V 7184 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7230 3600 50  0001 C CNN
F 3 "~" H 7300 3600 50  0001 C CNN
	1    7300 3600
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 61CA07A7
P 7300 3800
F 0 "R3" V 7093 3800 50  0000 C CNN
F 1 "20R" V 7184 3800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7230 3800 50  0001 C CNN
F 3 "~" H 7300 3800 50  0001 C CNN
	1    7300 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 61CA0BB2
P 7000 3900
F 0 "R1" V 6793 3900 50  0000 C CNN
F 1 "20R" V 6884 3900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6930 3900 50  0001 C CNN
F 3 "~" H 7000 3900 50  0001 C CNN
	1    7000 3900
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 61CA0F2C
P 7300 4000
F 0 "R4" V 7093 4000 50  0000 C CNN
F 1 "20R" V 7184 4000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 7230 4000 50  0001 C CNN
F 3 "~" H 7300 4000 50  0001 C CNN
	1    7300 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 4000 6750 4000
Wire Wire Line
	6750 3900 6850 3900
Wire Wire Line
	6750 3800 7150 3800
Wire Wire Line
	6750 3600 7150 3600
Wire Wire Line
	7450 3600 7550 3600
Wire Wire Line
	7150 3900 7550 3900
Wire Wire Line
	7450 3800 7550 3800
Wire Wire Line
	7450 4000 7550 4000
Wire Wire Line
	6750 3700 7550 3700
$Sheet
S 1050 2650 700  500 
U 61D60973
F0 "Channel LEDs" 50
F1 "Channel_LEDs.sch" 50
$EndSheet
$Sheet
S 1050 1900 700  500 
U 61F0493E
F0 "Channel Buttons And Switches" 50
F1 "Channel_Buttons_And_Switches.sch" 50
$EndSheet
$Sheet
S 1050 3400 700  500 
U 61FB496B
F0 "Channel Pots And Sliders" 50
F1 "Channel_Pots_And_Sliders.sch" 50
$EndSheet
$Sheet
S 1050 4200 700  500 
U 61FB9D3C
F0 "Channel ADCs" 50
F1 "Channel_ADCs.sch" 50
$EndSheet
Wire Wire Line
	7450 1900 7550 1900
Wire Wire Line
	7550 2000 7450 2000
Wire Wire Line
	7550 2600 7450 2600
Wire Wire Line
	7450 1200 7550 1200
Text GLabel 7450 4700 0    50   Input ~ 0
BTTN_UP_A
Text GLabel 7450 4600 0    50   Input ~ 0
BTTN_DOWN_A
Text GLabel 7450 4800 0    50   Input ~ 0
BTTN_PLAY_A
Wire Wire Line
	7550 4800 7450 4800
Wire Wire Line
	7450 4700 7550 4700
Wire Wire Line
	7550 4600 7450 4600
NoConn ~ 7550 2100
NoConn ~ 7550 2200
NoConn ~ 7550 3200
NoConn ~ 7550 3300
NoConn ~ 7550 3400
NoConn ~ 7550 3500
NoConn ~ 7550 4100
NoConn ~ 7550 4200
NoConn ~ 7550 4300
NoConn ~ 7550 4400
NoConn ~ 7550 4500
NoConn ~ 7550 4900
$EndSCHEMATC

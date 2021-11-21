EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector:TestPoint TP1
U 1 1 62937CBF
P 5750 3050
F 0 "TP1" H 5808 3168 50  0000 L CNN
F 1 "TestPoint" H 5808 3077 50  0000 L CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 5950 3050 50  0001 C CNN
F 3 "~" H 5950 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 629380B4
P 5750 3050
F 0 "#PWR0101" H 5750 2800 50  0001 C CNN
F 1 "GND" H 5755 2877 50  0000 C CNN
F 2 "" H 5750 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
Text Notes 5100 3550 0    50   ~ 0
This is just here so that we can have a GND net. \nOtherwise we wouldn’t be able to create copper\nfills on the frontpanel.
$EndSCHEMATC

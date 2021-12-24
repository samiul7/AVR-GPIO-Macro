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
L RF_Module:ESP32-WROOM-32D U?
U 1 1 61B83981
P 3200 4850
F 0 "U?" H 3300 6300 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 3650 6200 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 3200 3350 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 2900 4900 50  0001 C CNN
	1    3200 4850
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 61B8609A
P 2350 1300
F 0 "U?" H 2350 1542 50  0000 C CNN
F 1 "AMS1117-3.3" H 2350 1451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2350 1500 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2450 1050 50  0001 C CNN
	1    2350 1300
	1    0    0    -1  
$EndComp
$Comp
L semi-diode-generic:SS14 D?
U 1 1 61B8753E
P 1650 1300
F 0 "D?" V 1867 1300 50  0000 C CNN
F 1 "SS14" V 1776 1300 50  0000 C CNN
F 2 "IPC7351-Nominal:DIOM5326X292" H 1650 1150 50  0001 C CNN
F 3 "" H 1650 1300 60  0000 C CNN
F 4 "SCHOTTKY SS14" H 1650 1050 60  0001 C CNN "BOM"
	1    1650 1300
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 61B8B0B5
P 2800 1200
F 0 "#PWR?" H 2800 1050 50  0001 C CNN
F 1 "+3V3" H 2815 1373 50  0000 C CNN
F 2 "" H 2800 1200 50  0001 C CNN
F 3 "" H 2800 1200 50  0001 C CNN
	1    2800 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61B8B773
P 1450 2100
F 0 "#PWR?" H 1450 1850 50  0001 C CNN
F 1 "GND" H 1455 1927 50  0000 C CNN
F 2 "" H 1450 2100 50  0001 C CNN
F 3 "" H 1450 2100 50  0001 C CNN
	1    1450 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61B8C12F
P 2800 1650
F 0 "C?" H 2915 1696 50  0000 L CNN
F 1 "10uF" H 2915 1605 50  0000 L CNN
F 2 "" H 2838 1500 50  0001 C CNN
F 3 "~" H 2800 1650 50  0001 C CNN
	1    2800 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1300 2800 1300
$Comp
L Device:LED D?
U 1 1 61B8DD00
P 3400 1850
F 0 "D?" V 3439 1732 50  0000 R CNN
F 1 "PWR" V 3348 1732 50  0000 R CNN
F 2 "" H 3400 1850 50  0001 C CNN
F 3 "~" H 3400 1850 50  0001 C CNN
	1    3400 1850
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61B91849
P 1450 1200
F 0 "#PWR?" H 1450 1050 50  0001 C CNN
F 1 "+5V" H 1465 1373 50  0000 C CNN
F 2 "" H 1450 1200 50  0001 C CNN
F 3 "" H 1450 1200 50  0001 C CNN
	1    1450 1200
	1    0    0    -1  
$EndComp
Connection ~ 1900 1300
Wire Wire Line
	1900 1300 2050 1300
Wire Wire Line
	1750 1300 1900 1300
$Comp
L Device:R R?
U 1 1 61B8D4E3
P 3400 1450
F 0 "R?" H 3470 1496 50  0000 L CNN
F 1 "1K" H 3470 1405 50  0000 L CNN
F 2 "" V 3330 1450 50  0001 C CNN
F 3 "~" H 3400 1450 50  0001 C CNN
	1    3400 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61B888E0
P 1900 1650
F 0 "C?" H 2015 1696 50  0000 L CNN
F 1 "10uF" H 2015 1605 50  0000 L CNN
F 2 "" H 1938 1500 50  0001 C CNN
F 3 "~" H 1900 1650 50  0001 C CNN
	1    1900 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 61B9824B
P 1200 1700
F 0 "J?" H 1118 1375 50  0000 C CNN
F 1 "5V IN" H 1118 1466 50  0000 C CNN
F 2 "" H 1200 1700 50  0001 C CNN
F 3 "~" H 1200 1700 50  0001 C CNN
	1    1200 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 1300 1550 1300
Wire Wire Line
	1450 1200 1450 1300
Wire Wire Line
	2350 1600 2350 2000
Connection ~ 2350 2000
Wire Wire Line
	2350 2000 2800 2000
Wire Wire Line
	1450 2000 1450 2100
Wire Wire Line
	1400 1700 1450 1700
Wire Wire Line
	1450 1700 1450 2000
Connection ~ 1450 2000
Wire Wire Line
	1400 1600 1450 1600
Wire Wire Line
	1450 1600 1450 1300
Connection ~ 1450 1300
Wire Wire Line
	1450 2000 1900 2000
Wire Wire Line
	1900 1300 1900 1500
Wire Wire Line
	1900 1800 1900 2000
Connection ~ 1900 2000
Wire Wire Line
	1900 2000 2350 2000
Wire Wire Line
	2800 2000 2800 1800
Wire Wire Line
	2800 1300 2800 1500
Wire Wire Line
	2800 1200 2800 1300
Connection ~ 2800 1300
$Comp
L power:+5V #PWR?
U 1 1 61B9F72D
P 3400 1200
F 0 "#PWR?" H 3400 1050 50  0001 C CNN
F 1 "+5V" H 3415 1373 50  0000 C CNN
F 2 "" H 3400 1200 50  0001 C CNN
F 3 "" H 3400 1200 50  0001 C CNN
	1    3400 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61B9FC1A
P 3400 2100
F 0 "#PWR?" H 3400 1850 50  0001 C CNN
F 1 "GND" H 3405 1927 50  0000 C CNN
F 2 "" H 3400 2100 50  0001 C CNN
F 3 "" H 3400 2100 50  0001 C CNN
	1    3400 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1600 3400 1700
Wire Wire Line
	3400 1200 3400 1300
Wire Wire Line
	3400 2000 3400 2100
$Comp
L Switch:SW_Push SW?
U 1 1 61C40B8F
P 6650 1700
F 0 "SW?" H 6650 1985 50  0000 C CNN
F 1 "RESET" H 6650 1894 50  0000 C CNN
F 2 "" H 6650 1900 50  0001 C CNN
F 3 "~" H 6650 1900 50  0001 C CNN
	1    6650 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C41418
P 6200 1550
F 0 "R?" H 6270 1596 50  0000 L CNN
F 1 "10K" H 6270 1505 50  0000 L CNN
F 2 "" V 6130 1550 50  0001 C CNN
F 3 "~" H 6200 1550 50  0001 C CNN
	1    6200 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 1700 6400 1700
Wire Wire Line
	6400 2150 6200 2150
Wire Wire Line
	6200 2150 6200 1700
$Comp
L power:+5V #PWR?
U 1 1 61C42280
P 3400 1200
F 0 "#PWR?" H 3400 1050 50  0001 C CNN
F 1 "+5V" H 3415 1373 50  0000 C CNN
F 2 "" H 3400 1200 50  0001 C CNN
F 3 "" H 3400 1200 50  0001 C CNN
	1    3400 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 61C422CA
P 6200 1250
F 0 "#PWR?" H 6200 1100 50  0001 C CNN
F 1 "+3V3" H 6215 1423 50  0000 C CNN
F 2 "" H 6200 1250 50  0001 C CNN
F 3 "" H 6200 1250 50  0001 C CNN
	1    6200 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 1250 6200 1400
$Comp
L power:GND #PWR?
U 1 1 61C42FFA
P 6900 2200
F 0 "#PWR?" H 6900 1950 50  0001 C CNN
F 1 "GND" H 6905 2027 50  0000 C CNN
F 2 "" H 6900 2200 50  0001 C CNN
F 3 "" H 6900 2200 50  0001 C CNN
	1    6900 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1700 6900 1700
Wire Wire Line
	6200 2150 6000 2150
Connection ~ 6200 2150
Wire Wire Line
	6900 1700 6900 2200
Wire Wire Line
	6400 2150 6400 1700
Text Label 6000 2150 0    50   ~ 0
RES
$Comp
L Switch:SW_Push SW?
U 1 1 61C46FD7
P 8050 1700
F 0 "SW?" H 8050 1985 50  0000 C CNN
F 1 "BOOT" H 8050 1894 50  0000 C CNN
F 2 "" H 8050 1900 50  0001 C CNN
F 3 "~" H 8050 1900 50  0001 C CNN
	1    8050 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C46FE1
P 7600 1550
F 0 "R?" H 7670 1596 50  0000 L CNN
F 1 "10K" H 7670 1505 50  0000 L CNN
F 2 "" V 7530 1550 50  0001 C CNN
F 3 "~" H 7600 1550 50  0001 C CNN
	1    7600 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1700 7800 1700
Wire Wire Line
	7800 2150 7600 2150
Wire Wire Line
	7600 2150 7600 1700
$Comp
L power:+3V3 #PWR?
U 1 1 61C46FEE
P 7600 1250
F 0 "#PWR?" H 7600 1100 50  0001 C CNN
F 1 "+3V3" H 7615 1423 50  0000 C CNN
F 2 "" H 7600 1250 50  0001 C CNN
F 3 "" H 7600 1250 50  0001 C CNN
	1    7600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1250 7600 1400
$Comp
L power:GND #PWR?
U 1 1 61C46FF9
P 8300 2200
F 0 "#PWR?" H 8300 1950 50  0001 C CNN
F 1 "GND" H 8305 2027 50  0000 C CNN
F 2 "" H 8300 2200 50  0001 C CNN
F 3 "" H 8300 2200 50  0001 C CNN
	1    8300 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1700 8300 1700
Wire Wire Line
	7600 2150 7400 2150
Connection ~ 7600 2150
Wire Wire Line
	8300 1700 8300 2200
Wire Wire Line
	7800 2150 7800 1700
Text Label 7400 2150 0    50   ~ 0
BOOT
$Comp
L Device:LED D?
U 1 1 61C51B3A
P 3900 1850
F 0 "D?" V 3939 1732 50  0000 R CNN
F 1 "DBG" V 3848 1732 50  0000 R CNN
F 2 "" H 3900 1850 50  0001 C CNN
F 3 "~" H 3900 1850 50  0001 C CNN
	1    3900 1850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 61C51B40
P 3900 1450
F 0 "R?" H 3970 1496 50  0000 L CNN
F 1 "1K" H 3970 1405 50  0000 L CNN
F 2 "" V 3830 1450 50  0001 C CNN
F 3 "~" H 3900 1450 50  0001 C CNN
	1    3900 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61C51B4C
P 3900 2100
F 0 "#PWR?" H 3900 1850 50  0001 C CNN
F 1 "GND" H 3905 1927 50  0000 C CNN
F 2 "" H 3900 2100 50  0001 C CNN
F 3 "" H 3900 2100 50  0001 C CNN
	1    3900 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1600 3900 1700
Wire Wire Line
	3900 2000 3900 2100
Wire Wire Line
	3900 1150 3750 1150
Wire Wire Line
	3900 1150 3900 1300
Wire Wire Line
	5650 6600 5350 6600
Wire Wire Line
	5650 6700 5350 6700
$Comp
L power:GND #PWR?
U 1 1 61C5615B
P 5500 7100
F 0 "#PWR?" H 5500 6850 50  0001 C CNN
F 1 "GND" H 5505 6927 50  0000 C CNN
F 2 "" H 5500 7100 50  0001 C CNN
F 3 "" H 5500 7100 50  0001 C CNN
	1    5500 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 7100 5500 7000
Wire Wire Line
	5500 7000 5650 7000
Text Label 5350 6600 0    50   ~ 0
RX
Text Label 5350 6700 0    50   ~ 0
TX
NoConn ~ 5650 6900
NoConn ~ 5650 6500
$Comp
L Connector:Screw_Terminal_01x06 J?
U 1 1 61C59614
P 5850 6700
F 0 "J?" H 5930 6692 50  0000 L CNN
F 1 "Screw_Terminal_01x06" H 5930 6601 50  0000 L CNN
F 2 "" H 5850 6700 50  0001 C CNN
F 3 "~" H 5850 6700 50  0001 C CNN
	1    5850 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 6800 5500 6800
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 61C5CD00
P 5500 6300
F 0 "JP?" V 5454 6368 50  0000 L CNN
F 1 "Solder Jumper" V 5545 6368 50  0000 L CNN
F 2 "" H 5500 6300 50  0001 C CNN
F 3 "~" H 5500 6300 50  0001 C CNN
	1    5500 6300
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 61C5E82B
P 5500 6050
F 0 "#PWR?" H 5500 5900 50  0001 C CNN
F 1 "+3V3" H 5515 6223 50  0000 C CNN
F 2 "" H 5500 6050 50  0001 C CNN
F 3 "" H 5500 6050 50  0001 C CNN
	1    5500 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 6050 5500 6150
Wire Wire Line
	5500 6450 5500 6800
$Comp
L power:GND #PWR?
U 1 1 61C60C96
P 3200 6250
F 0 "#PWR?" H 3200 6000 50  0001 C CNN
F 1 "GND" H 3205 6077 50  0000 C CNN
F 2 "" H 3200 6250 50  0001 C CNN
F 3 "" H 3200 6250 50  0001 C CNN
	1    3200 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C?
U 1 1 61C61664
P 1300 4000
F 0 "C?" H 1415 4046 50  0000 L CNN
F 1 "100uF/6.3V" H 1415 3955 50  0000 L CNN
F 2 "" H 1300 4000 50  0001 C CNN
F 3 "~" H 1300 4000 50  0001 C CNN
	1    1300 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 61C62A71
P 1300 3650
F 0 "#PWR?" H 1300 3500 50  0001 C CNN
F 1 "+3V3" H 1315 3823 50  0000 C CNN
F 2 "" H 1300 3650 50  0001 C CNN
F 3 "" H 1300 3650 50  0001 C CNN
	1    1300 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61C6344A
P 1300 4350
F 0 "#PWR?" H 1300 4100 50  0001 C CNN
F 1 "GND" H 1305 4177 50  0000 C CNN
F 2 "" H 1300 4350 50  0001 C CNN
F 3 "" H 1300 4350 50  0001 C CNN
	1    1300 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3650 1300 3850
Wire Wire Line
	1300 4150 1300 4350
$Comp
L power:+3V3 #PWR?
U 1 1 61C67C26
P 3200 3450
F 0 "#PWR?" H 3200 3300 50  0001 C CNN
F 1 "+3V3" H 3215 3623 50  0000 C CNN
F 2 "" H 3200 3450 50  0001 C CNN
F 3 "" H 3200 3450 50  0001 C CNN
	1    3200 3450
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-Bipolar-BJT-Single:2N3904 Q?
U 1 1 61C6C454
P 7800 3950
F 0 "Q?" H 7988 4003 60  0000 L CNN
F 1 "2N3904" H 7988 3897 60  0000 L CNN
F 2 "digikey-footprints:TO-92-3" H 8000 4150 60  0001 L CNN
F 3 "https://my.centralsemi.com/get_document.php?cmp=1&mergetype=pd&mergepath=pd&pdf_id=LSSGP072.PDF" H 8000 4250 60  0001 L CNN
F 4 "2N3904CS-ND" H 8000 4350 60  0001 L CNN "Digi-Key_PN"
F 5 "2N3904" H 8000 4450 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 8000 4550 60  0001 L CNN "Category"
F 7 "Transistors - Bipolar (BJT) - Single" H 8000 4650 60  0001 L CNN "Family"
F 8 "https://my.centralsemi.com/get_document.php?cmp=1&mergetype=pd&mergepath=pd&pdf_id=LSSGP072.PDF" H 8000 4750 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/central-semiconductor-corp/2N3904/2N3904CS-ND/4806876" H 8000 4850 60  0001 L CNN "DK_Detail_Page"
F 10 "TRANS NPN 40V TO-92" H 8000 4950 60  0001 L CNN "Description"
F 11 "Central Semiconductor Corp" H 8000 5050 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8000 5150 60  0001 L CNN "Status"
	1    7800 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C6D2EC
P 7350 4250
F 0 "R?" H 7420 4296 50  0000 L CNN
F 1 "10K" H 7420 4205 50  0000 L CNN
F 2 "" V 7280 4250 50  0001 C CNN
F 3 "~" H 7350 4250 50  0001 C CNN
	1    7350 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61C6DBA5
P 7000 3950
F 0 "R?" V 6900 3900 50  0000 L CNN
F 1 "1K" V 7100 3900 50  0000 L CNN
F 2 "" V 6930 3950 50  0001 C CNN
F 3 "~" H 7000 3950 50  0001 C CNN
	1    7000 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 3950 7350 3950
Wire Wire Line
	6850 3950 6650 3950
Connection ~ 7350 3950
Wire Wire Line
	7350 3950 7600 3950
Wire Wire Line
	7350 3950 7350 4100
Wire Wire Line
	7350 4400 7350 4500
Wire Wire Line
	7350 4500 7900 4500
Wire Wire Line
	7900 4500 7900 4150
$Comp
L power:GND #PWR?
U 1 1 61C72A35
P 7900 4600
F 0 "#PWR?" H 7900 4350 50  0001 C CNN
F 1 "GND" H 7905 4427 50  0000 C CNN
F 2 "" H 7900 4600 50  0001 C CNN
F 3 "" H 7900 4600 50  0001 C CNN
	1    7900 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 4600 7900 4500
Connection ~ 7900 4500
$Comp
L Diode:SM4007 D?
U 1 1 61C745F1
P 7900 3450
F 0 "D?" V 7854 3530 50  0000 L CNN
F 1 "SM4007" V 7945 3530 50  0000 L CNN
F 2 "Diode_SMD:D_MELF" H 7900 3275 50  0001 C CNN
F 3 "http://cdn-reichelt.de/documents/datenblatt/A400/SMD1N400%23DIO.pdf" H 7900 3450 50  0001 C CNN
	1    7900 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	7900 3100 7900 3300
Wire Wire Line
	7900 3600 7900 3750
$Comp
L dk_Automotive-Relays:JSM1-12V-5 RLY?
U 1 1 61C7D8AC
P 8650 3400
F 0 "RLY?" H 8700 3737 60  0000 C CNN
F 1 "SDR-05VDC-SL-C" H 8700 3631 60  0000 C CNN
F 2 "digikey-footprints:Relay_JSM1-12V-5" H 8850 3600 60  0001 L CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Panasonic%20Electric%20Works%20PDFs/JS-M_Series_Rev_Dec_2011.pdf" H 8850 3700 60  0001 L CNN
F 4 "255-1240-ND" H 8850 3800 60  0001 L CNN "Digi-Key_PN"
F 5 "JSM1-12V-5" H 8850 3900 60  0001 L CNN "MPN"
F 6 "Relays" H 8850 4000 60  0001 L CNN "Category"
F 7 "Automotive Relays" H 8850 4100 60  0001 L CNN "Family"
F 8 "https://media.digikey.com/pdf/Data%20Sheets/Panasonic%20Electric%20Works%20PDFs/JS-M_Series_Rev_Dec_2011.pdf" H 8850 4200 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/panasonic-electric-works/JSM1-12V-5/255-1240-ND/274764" H 8850 4300 60  0001 L CNN "DK_Detail_Page"
F 10 "RELAY AUTOMOTIVE SPDT 15A 12V" H 8850 4400 60  0001 L CNN "Description"
F 11 "Panasonic Electric Works" H 8850 4500 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8850 4600 60  0001 L CNN "Status"
	1    8650 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3300 7900 3300
Connection ~ 7900 3300
Wire Wire Line
	8350 3600 7900 3600
Connection ~ 7900 3600
$Comp
L power:+5V #PWR?
U 1 1 61C85968
P 7900 3100
F 0 "#PWR?" H 7900 2950 50  0001 C CNN
F 1 "+5V" H 7915 3273 50  0000 C CNN
F 2 "" H 7900 3100 50  0001 C CNN
F 3 "" H 7900 3100 50  0001 C CNN
	1    7900 3100
	1    0    0    -1  
$EndComp
Connection ~ 3400 1200
$EndSCHEMATC

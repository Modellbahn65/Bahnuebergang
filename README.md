# Bahnübergang

Eine superrealistische Simulation
eines DB-Bahnüberganges
mit Halogen-Lampen
und Quecksilber-Thermoblinkers.

Ausgelegt für einen Bahnübergang mit zwei/vier beleuchteten Warnschildern
und zwei Überwachersignalen mit jeweils einer weißen LED oben und einer orangenen LED unten.

* Arbeitet als DCC-Accessory-Decoder um Schaltbefehle direkt von der Zentrale zu empfangen
* Simuliertes Auf- und Abdimmen von Halogenlampen
* Simulierte Imperfektion eines Quecksilber-Thermoblinkers
  ([nach Spezifikation](https://web.archive.org/web/20240830160709/https://mecl.ch/images/content/2019/Quecksilber/Quecksilberblinker.pdf))
* 10-bit PWM für superweiches Dimmen der LEDs
* Implementierte Gamma-Kurve für realistische Helligkeitsverläufe

## Setup

Das Projekt wird auf einem [Sparkfun Pro Micro](https://www.sparkfun.com/products/12640) ausgeführt.

Folgende Pins werden verwendet:
Pin|Beschreibung
-|-
`RAW` | `5V` Spannungsversorgung
`GND` | Masse
`RST` | Ein optionaler Taster zwischen `GND` und `RST` erlaubt einfaches neustarten des Mikrocontrollers
`2` | [Optokoppeltes DCC Signal](https://www.modellbahn-digitaltechnik.de/wp-content/uploads/2020/06/DCCanMikrocontroller.jpg)
`3` | orangene LEDs beider Überwachersignale
`5` | weiße LED Überwachersignal 1
`6` | weiße LED Überwachersignal 2
`9` | Bahnübergang Phase A
`10` | Bahnübergang Phase B

Alle LEDs sollen leuchten wenn der zugehörige Pin auf `5V` hochgezogen wird.
Hierfür können die LEDs direkt mit Widerstand an den Mikrocontroller angeschlossen werden,
da dieser die benötigten `20mA` pro Pin liefern kann.

## Benutzung

In [dcc.cpp](src/dcc.cpp) definiert man die `DCC_BOARD_ADDRESS` auf die gewünschte Addresse.
Die verwendeten vier Weichenaddressen kann man mit `(DCC_BOARD_ADDRESS - 1) * 4 + 1` errechnen,
sie werden der Einfachheit halber aber auch bein Start auf der seriellen Konsole ausgegeben.

Adresse | Funktion
-|-
`+0` | Weißes Blinklicht Überwachersignal 1
`+1` | Weißes Blinklicht Überwachersignal 2
`+2` | Orangene Lichter beider Überwachersignale
`+3` | rotes Blinken Bahnübergang

***Anmerkung:** Das Ausschalten der orangenen Lichter bewirkt auch das Ausschalten der weißen Blinklichter.*

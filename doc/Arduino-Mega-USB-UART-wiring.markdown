# Arduino Mega USB UART converter wiring

## Introduction

This wiring schema uses only Tx from Arduino and is suitable to be used as standard error console.

## Wiring illustration

![Arduino Mega USB UART wiring.png](Arduino-Mega-USB-UART-wiring.png)

## Wiring table

| Signal | ATMega2560 port and pin | Arduino Mega 2560 pin | USB UART converter pin |
| --- | --- | --- | --- |
| Ground (GND) | - | GND | GND |
| Transmit data  from Arduino (TxD) | PORTJ 1 (TXD3) | 14 (TX3) | TxD |


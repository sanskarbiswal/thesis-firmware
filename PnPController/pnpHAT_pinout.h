#ifndef _PNPHAT_PINOOUT_H
#define _PNPHAT_PINOOUT_H

// Raspberry Pi UART Connection
const int PI_TX = 12;
const int PI_RX = 13;

// Motor 1
const int M1_DIR = 2;
const int M1_STEP = 3;
const int M1_UART_TX = 4;
const int M1_UART_RX = 5;
const int M1_EN =  19;

// Motor 2
const int M2_STEP = 6;
const int M2_DIR = 7;
const int M2_UART_TX = 8;
const int M2_UART_RX = 9;
const int M2_EN =  20;

// Motor 3 TODO: Fix GPIO pinout
const int M3_STEP = 19;
const int M3_DIR = 20;
const int M3_UART_TX = 21;
const int M3_UART_RX = 22;
// Motor 3 is always enabled

// Solenoid Air Valve Control
const int VALVE_CTLR = 22;

// Control Pins from RPi GPIO
const int S0_PIN = 26;
const int S1_PIN = 27;
const int S2_PIN = 28;

const int S_INT =  21;    // Interrupt Notification

#endif
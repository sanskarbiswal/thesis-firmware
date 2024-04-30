#ifndef _PNPHAT_PINOOUT_H
#define _PNPHAT_PINOOUT_H

// Raspberry Pi UART Connection
#define PI_TX             16
#define PI_RX             17

// Motor 1
#define M1_DIR            4
#define M1_STEP           5
#define M1_UART_TX        6
#define M1_UART_RX        7
#define M1_EN             25

// Motor 2
#define M2_STEP           9
#define M2_DIR            10
#define M2_UART_TX        11
#define M2_UART_RX        12
#define M2_EN             26

// Motor 3
#define M3_STEP           19
#define M3_DIR            20
#define M3_UART_TX        21
#define M3_UART_RX        22
// Motor 3 is always enabled

// Solenoid Air Valve Control
#define VALVE_CTLR        29

// Control Pins from RPi GPIO
#define S0_PIN            31
#define S1_PIN            32
#define S2_PIN            34

#define S_INT             27    // Interrupt Notification

#endif
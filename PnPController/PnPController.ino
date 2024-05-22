#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <SpeedyStepper.h>
#include <TMCStepper.h>

#include "pnpHAT_pinout.h"

#define STEPS_PER_REVOLUTION    200
#define R_SENSE                 0.11f

#define MOTOR_MICROSTEP         16
#define MOTOR_CURRENT           1000

#define MOTOR_1                 // Z-Alignment Stepper Motor
// #define MOTOR_2                 // Hollow-Shaft Stepper Motor

#ifdef MOTOR_1
SoftwareSerial m1_uart(M1_UART_RX, M1_UART_TX); // RX, TX
TMC2209Stepper driver1(&m1_uart, R_SENSE, 0b00);
SpeedyStepper stepper1;
#endif

#ifdef MOTOR_2
SoftwareSerial m2_uart(M2_UART_RX, M2_UART_TX); // RX, TX
TMC2209Stepper driver2(&m2_uart, R_SENSE, 0b01);
SpeedyStepper stepper2;
#endif

#ifdef MOTOR_3
SoftwareSerial m3_uart(M3_UART_RX, M3_UART_TX); // RX, TX
TMC2209Stepper driver3(&m3_uart, R_SENSE, 0b10);
SpeedyStepper stepper3;
#endif

// ------------------ Function Declaration -------------------- //
void init_stepper_motors(void);
void enable_motor(int);
void disable_motor(int);
void valve_ON(void);
void valve_OFF(void);
void rotate_motor1_by_angle(float);
void rotate_motor2_by_angle(float);


/**
 * Function to Initialize Stepper Motor
*/
void init_stepper_motors(){
  #ifdef MOTOR_1
  pinMode(M1_STEP, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_EN, OUTPUT);

  digitalWrite(M1_EN, HIGH); // Hardware Enable 
  stepper1.connectToPins(M1_STEP, M1_DIR);

  driver1.begin();
  driver1.toff(5);          // Enables Driver in Software
  driver1.rms_current(MOTOR_CURRENT);
  driver1.microsteps(MOTOR_MICROSTEP);

  // driver1.pwm_autoscale(true);    // Needed for stealthChop
  // driver1.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
  driver1.en_spreadCycle(false);

  stepper1.setCurrentPositionInSteps(0);
  stepper1.setSpeedInStepsPerSecond(400);
  stepper1.setAccelerationInStepsPerSecondPerSecond(400);
  #endif
  
  #ifdef MOTOR_2
  pinMode(M2_STEP, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_EN, OUTPUT);

  pinMode(M2_EN, HIGH); // Hardware Enable 
  stepper2.connectToPins(M2_STEP, M2_DIR);

  driver2.begin();
  driver2.toff(5);          // Enables Driver in Software
  driver2.rms_current(MOTOR_CURRENT);
  driver2.microsteps(MOTOR_MICROSTEP);

  // driver2.pwm_autoscale(true);    // Needed for stealthChop
  // driver2.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
  driver2.en_spreadCycle(false);

  stepper2.setCurrentPositionInSteps(0);
  stepper2.setSpeedInStepsPerSecond(400);
  stepper2.setAccelerationInStepsPerSecondPerSecond(400);
  #endif

  #ifdef MOTOR_3
  pinMode(M3_STEP, OUTPUT);
  pinMode(M3_DIR, OUTPUT);
  pinMode(M3_EN, OUTPUT);

  stepper3.connectToPins(M3_STEP, M3_DIR);

  driver3.begin();
  driver3.toff(5);          // Enables Driver in Software
  driver3.rms_current(MOTOR_CURRENT);
  driver3.microsteps(MOTOR_MICROSTEP);

  // driver3.pwm_autoscale(true);    // Needed for stealthChop
  // driver3.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
  driver3.en_spreadCycle(false);

  stepper3.setCurrentPositionInSteps(0);
  stepper3.setSpeedInStepsPerSecond(400);
  stepper3.setAccelerationInStepsPerSecondPerSecond(400);
  #endif
}


/*
 * Function to Hardware Enable Motor
 * GPIO Control - Pull-up at MCU
*/
void enable_motor(int motorID){
  switch(motorID){
    case 1:
      pinMode(M1_EN, LOW);
      break;
    case 2:
      pinMode(M2_EN, LOW);
      break;
  }
}


/*
 * Function to Hardware Disable Motor
 * GPIO Control - Pull-up at MCU
*/
void disable_motor(int motorID){
  switch(motorID){
    case 1:
      pinMode(M1_EN, HIGH);
      break;
    case 2:
      pinMode(M2_EN, HIGH);
      break;
  }
}

/*
 * Function to Turn ON Valve
 * NMOS Control - Pulled-down at Gate
*/
void valve_ON(){
  pinMode(VALVE_CTLR, HIGH);
}

/*
 * Function to Turn OFF Valve
 * NMOS Control - Pulled-down at Gate
*/
void valve_OFF(){
  pinMode(VALVE_CTLR, LOW);
}

/*
 * Funtion to rotate Motor 1 by param angle
 * param[in] angle
*/
void rotate_motor1_by_angle(float angle){
  long curr_pos = stepper1.getCurrentPositionInSteps();
  long target_pos = curr_pos - (angle * STEPS_PER_REVOLUTION/360);
  stepper1.moveToPositionInSteps(target_pos);
}

void init_serial(){
  Serial1.setRX(17u);
  Serial1.setTX(16u);
  Serial1.begin(9600);

  m1_uart.begin(9600);
  #ifdef MOTOR_2
  m2_uart.begin(9600);
  #endif
  delay(100);
}


// Function to Handle Interrupt from S_INT
void handle_piINTR(){

}

void init_intr_pins(){
  pinMode(S_INT, INPUT_PULLDOWN);
  attachInterrupt(S_INT, handle_piINTR, RISING);

  interrupts();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VALVE_CTLR, OUTPUT);

  init_serial();
  init_stepper_motors();
  enable_motor(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  valve_ON();
  rotate_motor1_by_angle(10);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  valve_OFF();
  rotate_motor1_by_angle(-10);
  delay(2000);

}


// Required Packages and Board
/*
 * Board
 --------------------
 * PICO             : https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

 * Library
 --------------------
 * SpeedyStepper    : https://github.com/Stan-Reifel/SpeedyStepper
 * TMCStepper       : https://github.com/teemuatlut/TMCStepper 
*/

#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <SpeedyStepper.h>
#include <TMCStepper.h>

#include "pnpHAT_pinout.h"

#define STEPS_PER_REVOLUTION    200
#define R_SENSE                 0.11f

#define MOTOR_MICROSTEP         16
#define MOTOR_CURRENT           300

#define MOTOR_1

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
void rotate_motor_by_anagle(SpeedyStepper, float);


/**
 * Function to Initialize Stepper Motor
*/
void init_stepper_motors(){
  #ifdef MOTOR_1
  pinMode(M1_STEP, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_EN, OUTPUT);

  pinMode(M1_EN, HIGH);
  stepper1.connectToPins(M1_STEP, M1_DIR);

  driver1.begin();
  driver1.toff(5);          // Enables Driver in Software
  driver1.rms_current(MOTOR_CURRENT);
  driver1.microsteps(MOTOR_MICROSTEP);

  // driver.pwm_autoscale(true);    // Needed for stealthChop
  // driver.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
  driver1.en_spreadCycle(false);

  stepper1.setCurrentPositionInSteps(0);
  stepper1.setSpeedInStepsPerSecond(400);
  stepper1.setAccelerationInStepsPerSecondPerSecond(400);
  #endif
}


/*
 * Function to Hardware Enable Motor
 * PMOS Contol
*/
void enable_motor(int motorID){
  switch(motorID){
    case 1:
      pinMode(M1_EN, HIGH);
      break;
    case 2:
      pinMode(M2_EN, HIGH);
      break;
  }
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

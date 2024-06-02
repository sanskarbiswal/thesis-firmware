#include<SpeedyStepper.h>


// ------------------------------------ GPIO Connections --------------------------------------//
// Raspberry Pi UART Connection
const int PI_TX = 12;
const int PI_RX = 13;

// Motor 1
const int M1_DIR = 2;
const int M1_STEP = 3;
// const int M1_UART_TX = 4;
// const int M1_UART_RX = 5;
const int M1_EN =  19;

// Motor 2
const int M2_STEP = 6;
const int M2_DIR = 7;
// const int M2_UART_TX = 8;
// const int M2_UART_RX = 9;
const int M2_EN =  20;

// Solenoid Air Valve Control
const int VALVE_CTLR = 22;

// Control Pins from RPi GPIO
const int S0_PIN = 26;    // Toggle Valve
const int S1_PIN = 27;    // Toggle RigPosition
const int S2_PIN = 28;

const int S_INT =  21;    // Interrupt Notification

// ------------------------ Global Variables ---------------------------------- //
// States affected by INTR from  RPi
volatile bool valveState = false;
volatile bool rigState = false;

// ------------------------- Definitions --------------------------------- //
#define STEPS_PER_REV                       200
#define R_SENSE                             0.11f

SpeedyStepper stepper1;
SpeedyStepper stepper2;

// ------------------------- Function Declaration -------------------------- //
void init_gpio(void);
void init_stepper_motors(void);
void enable_motor(int);
void disable_motor(int);
void valve_ON(void);
void valve_OFF(void);
void toggle_z_rig(void);


void init_gpio(){
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(M1_EN, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_STEP, OUTPUT);

  pinMode(M2_EN, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_STEP, OUTPUT);

  pinMode(VALVE_CTLR, OUTPUT);
}

void init_stepper_motors(){
  stepper1.connectToPins(M1_STEP, M1_DIR);
  stepper1.setStepsPerMillimeter(12);
  stepper1.setSpeedInMillimetersPerSecond(50);
  stepper1.setAccelerationInMillimetersPerSecondPerSecond(20);
  digitalWrite(M1_EN, HIGH);
}

void enable_motor(int motorID){
  switch(motorID){
    case 1:
      digitalWrite(M1_EN, LOW);
      break;
    case 2:
      digitalWrite(M2_EN, LOW);
      break;
  }
}

void disable_motor(int motorID){
  switch(motorID){
    case 1:
      digitalWrite(M1_EN, HIGH);
      break;
    case 2:
      digitalWrite(M2_EN, HIGH);
      break;
  }
}

void valve_ON(){
  digitalWrite(VALVE_CTLR, HIGH);
}

void valve_OFF(){
  digitalWrite(VALVE_CTLR, LOW);;
}

void toggle_z_rig(){
  enable_motor(1);
  if (rigState){
    stepper1.moveToPositionInMillimeters(0);
    rigState = false;
    delay(100);
  } else {
    stepper1.moveToPositionInMillimeters(-510);
    rigState = true;
    delay(100);
  }
}

void setup() {
  // put your setup code here, to run once:
  init_gpio();
  init_stepper_motors();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  toggle_z_rig();
  digitalWrite(LED_BUILTIN, LOW);
  toggle_z_rig();
}

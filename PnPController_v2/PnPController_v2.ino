#include<SpeedyStepper.h>
#include<Arduino.h>

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

struct tool_status {
  bool valveState;
  bool rigState;
};

tool_status status = {false, false};

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
void toggle_valve(void);
void init_intr(void);
void handle_uart(String);
void transmitStatus(void);
void resumePrinterOps(void);
void test_motor_2(void);


void init_gpio(){
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(M1_EN, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_STEP, OUTPUT);

  pinMode(M2_EN, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_STEP, OUTPUT);

  pinMode(VALVE_CTLR, OUTPUT);

  pinMode(S0_PIN, INPUT_PULLDOWN);
  pinMode(S1_PIN, INPUT_PULLDOWN);
}

void init_stepper_motors(){
  stepper1.connectToPins(M1_STEP, M1_DIR);
  stepper1.setStepsPerMillimeter(12);
  stepper1.setSpeedInMillimetersPerSecond(50);
  stepper1.setAccelerationInMillimetersPerSecondPerSecond(20);
  digitalWrite(M1_EN, HIGH);

  stepper2.connectToPins(M2_STEP, M2_DIR);
  stepper2.setSpeedInStepsPerSecond(100);
  stepper2.setAccelerationInStepsPerSecondPerSecond(100);
  digitalWrite(M2_EN, HIGH);
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
  valveState = true;
}

void valve_OFF(){
  digitalWrite(VALVE_CTLR, LOW);
  valveState = false;
}

void toggle_valve(){
  if (valveState){
    valve_OFF();
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    valve_ON();
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void toggle_z_rig(){
  enable_motor(1);
  if (rigState){
    stepper1.moveToPositionInMillimeters(0);
    rigState = false;
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    stepper1.moveToPositionInMillimeters(-510);
    rigState = true;
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void init_intr(){
  attachInterrupt(S0_PIN, toggle_valve, FALLING);
  attachInterrupt(S1_PIN, toggle_z_rig, FALLING);
}

void handle_uart(String data){
  if (data.startsWith("301")){
    float angle = data.substring(4).toFloat();
    // Handle Angle Rotation
    int steps = angle;
    enable_motor(2);
    digitalWrite(LED_BUILTIN, HIGH);
    stepper2.moveRelativeInSteps(steps);
    delay(1000);
    disable_motor(2);
    digitalWrite(LED_BUILTIN, LOW);

  }else{
    int cmd = data.toInt();
    switch(cmd) {
      case 100:
        if (valveState) {
          valve_OFF();
          digitalWrite(LED_BUILTIN, LOW);
          valveState = false;
          status.valveState = valveState;
        }
        break;
      case 101:
        if (!valveState) {
          valve_ON();
          digitalWrite(LED_BUILTIN, HIGH);
          valveState = true;
          status.valveState = valveState;
        }
        break;
      
      case 200:
        if (rigState){
          rigState = false;
          enable_motor(1);
          stepper1.moveToPositionInMillimeters(0);
          delay(100);
          disable_motor(1);
          rigState = false;
          status.rigState = rigState;
        }
        break;

      case 201:
        if (!rigState){
          rigState = true;
          enable_motor(1);
          stepper1.moveToPositionInMillimeters(-510);
          delay(100);
          disable_motor(1);
          rigState = true;
          status.rigState = rigState;
        }
        break;

      case 900:
        transmitStatus();
        break;

      default:
        break;
    }
  }
}

void transmitStatus() {
  Serial.print("status ");
  Serial.print(status.valveState ? "1" : "0");
  Serial.print(status.rigState ? "1" : "0");
  Serial.println();
}

void resumePrinterOps(){
  Serial.println("RESUME");
}

void test_motor_2(){
  int angle = 60;
  int steps = (5/9)*angle;
  enable_motor(2);
  digitalWrite(LED_BUILTIN, HIGH);
  stepper2.moveRelativeInSteps(steps);
  disable_motor(2);
  digitalWrite(LED_BUILTIN, LOW);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_gpio();
  init_intr();
  init_stepper_motors();
  status.valveState = false;
  status.rigState = false;  
}

void loop() {
  // put your main code here, to run repeatedly:
  // delay(10);
  // Test Motor 2
  // test_motor_2();
  // delay(1000);
  // Check if data is available on USB Serial
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    handle_uart(data);
    transmitStatus();
    resumePrinterOps();
  }
}

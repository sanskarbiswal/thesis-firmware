#include<SpeedyStepper.h>
#include<Arduino.h>

// ------------------------------------ GPIO Connections --------------------------------------//
// Definitions for GPIO pins used for interfacing with motors, solenoid valves, and the Raspberry Pi.

// Raspberry Pi UART Connection
const int PI_TX = 12;  // UART TX pin for Raspberry Pi
const int PI_RX = 13;  // UART RX pin for Raspberry Pi

// Motor 1
const int M1_DIR = 2;      // Direction control pin for Motor 1
const int M1_STEP = 3;     // Step control pin for Motor 1
const int M1_EN =  19;     // Enable pin for Motor 1

// Motor 2
const int M2_STEP = 6;     // Step control pin for Motor 2
const int M2_DIR = 7;      // Direction control pin for Motor 2
const int M2_EN =  20;     // Enable pin for Motor 2

// Solenoid Air Valve Control
const int VALVE_CTLR = 22; // Control pin for solenoid valve

// Control Pins from RPi GPIO
const int S0_PIN = 26;     // Input pin for toggling the valve state
const int S1_PIN = 27;     // Input pin for toggling the rig position
const int S2_PIN = 28;     // Another control input pin (purpose not explicitly stated)

const int S_INT =  21;     // Interrupt notification pin

struct tool_status {
  bool valveState;         // Current state of the valve
  bool rigState;           // Current state of the rig
};

tool_status status = {false, false};

// ------------------------ Global Variables ---------------------------------- //
// States affected by interrupts from the RPi
volatile bool valveState = false;
volatile bool rigState = false;

// ------------------------- Definitions --------------------------------- //
#define STEPS_PER_REV 200     // Number of steps per revolution for the stepper motors
#define R_SENSE       0.11f   // Current sensing resistor value

SpeedyStepper stepper1;       // Stepper motor control object for Motor 1
SpeedyStepper stepper2;       // Stepper motor control object for Motor 2

// ------------------------- Function Declaration -------------------------- //
// Function prototypes for initialization, control, and handling tasks.
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

// ----------------------------------- Function Definitions ----------------------------------- //



/**
 * @brief Initializes all the GPIO pins used in the project.
 * This function sets up the pin modes for motor control, solenoid valve control, 
 * and input pins connected to the Raspberry Pi.
 */
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
  pinMode(S2_PIN, INPUT_PULLDOWN);
  pinMode(S_INT, OUTPUT);
}


/**
 * @brief Initializes the stepper motors.
 * This function sets up the stepper motors with their respective parameters 
 * such as steps per revolution.
 */
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


/**
 * @brief Enables a specified motor.
 * @param motorID ID of the motor to enable (1 or 2).
 */
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

/**
 * @brief Disables a specified motor.
 * @param motorID ID of the motor to disable (1 or 2).
 */
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


/**
 * @brief Turns the solenoid valve ON.
 * This function activates the solenoid valve by setting the control pin HIGH.
 */
void valve_ON(){
  digitalWrite(VALVE_CTLR, HIGH);
  valveState = true;
}

/**
 * @brief Turns the solenoid valve OFF.
 * This function deactivates the solenoid valve by setting the control pin LOW.
 */
void valve_OFF(){
  digitalWrite(VALVE_CTLR, LOW);
  valveState = false;
}


/**
 * @brief Toggles the state of the solenoid valve.
 * This function switches the solenoid valve between ON and OFF states.
 */
void toggle_valve(){
  if (valveState){
    valve_OFF();
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    valve_ON();
    digitalWrite(LED_BUILTIN, HIGH);
  }
}


/**
 * @brief Toggles the position of the Z-axis rig.
 * This function is likely used to move the Z-axis rig up or down based on the current state.
 */
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


/**
 * @brief Initializes the interrupt mechanism.
 * Sets up the interrupt service routine (ISR) to handle external interrupts 
 * triggered by the Raspberry Pi.
 */
void init_intr(){
  attachInterrupt(S0_PIN, toggle_valve, FALLING);
  attachInterrupt(S1_PIN, toggle_z_rig, FALLING);
}


/**
 * @brief Handles UART communication from the Raspberry Pi.
 * @param command The command string received via UART.
 * This function interprets the command received from the Raspberry Pi and 
 * executes the corresponding action.
 */
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


/**
 * @brief Transmits the current status of the tool back to the Raspberry Pi.
 * Sends a status update via UART to inform the Raspberry Pi of the current 
 * states of the valve and the rig.
 */
void transmitStatus() {
  Serial.print("status ");
  Serial.print(status.valveState ? "1" : "0");
  Serial.print(status.rigState ? "1" : "0");
  Serial.println();
}


/**
 * @brief Resumes the printer's operations after completing the pick-and-place task.
 * This function can be called to signal the printer to continue its tasks 
 * after the pick-and-place process is done.
 */
void resumePrinterOps(){
  Serial.println("RESUME");
}


/**
 * @brief A test function for Motor 2.
 * This function is used to manually test the operation of Motor 2 by moving 
 * it forward and backward.
 */
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

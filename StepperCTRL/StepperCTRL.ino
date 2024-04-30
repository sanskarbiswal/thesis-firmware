#include <TMCStepper.h>
#include <SpeedyStepper.h>
#include <SoftwareSerial.h>

// Define the number of steps per revolution of your stepper motors
#define STEPS_PER_REVOLUTION 200

// Define the angles you want to move each motor
#define ANGLE_1 30
#define ANGLE_2 45
#define ANGLE_3 60

#define R_SENSE 0.11f  // Matched according to Library Example

// Define the pin for the button
#define BUTTON_PIN 5

// Define Motor Current to use for driving
#define MOTOR_CURRENT_PERCENT 30
#define MOTOR_MICROSTEP 16

SoftwareSerial serial_stream(12, 13); // (RX, TX)
SoftwareSerial serial_stream2(10, 11);

// Define Motors in use
// Use definition to intialize motors later
#define MOTOR_1
#define MOTOR_2
// #define MOTOR_3

#ifdef MOTOR_1
  // Define the pins for the stepper motor drivers
  #define DIR_PIN_1         2
  #define STEP_PIN_1        3
  #define EN_PIN_1          4
  #define M1_ADDR           0b00
  // Create TMC2209Stepper objects for each motor
  // TMC2209 motor1;
  TMC2209Stepper driver1(&serial_stream, R_SENSE, M1_ADDR);
  SpeedyStepper stepper1;
  bool shatf1 = false;
#endif

#ifdef MOTOR_2
  #define DIR_PIN_2 6
  #define STEP_PIN_2 7
  #define EN_PIN_2 8
  #define M2_ADDR     0b01
  // Create TMC2209Stepper objects for each motor
  TMC2209Stepper driver2(&serial_stream2, R_SENSE, M2_ADDR);
  SpeedyStepper stepper2;
  bool shaft2 = false;
#endif

// #ifdef MOTOR_3
//   #define DIR_PIN_3 10
//   #define STEP_PIN_3 11
//   #define EN_PIN_3 12
//   // Create TMC2209Stepper objects for each motor
//   TMC2209 motor3;
// #endif

/**
  function to initialize motors
**/
void init_steppers() {
  serial_stream.begin(4800);
  #ifdef MOTOR_1
    pinMode(STEP_PIN_1, OUTPUT);
    pinMode(DIR_PIN_1, OUTPUT);
    pinMode(EN_PIN_1, OUTPUT);
    digitalWrite(EN_PIN_1, HIGH);  // Hardware Enable

    stepper1.connectToPins(STEP_PIN_1, DIR_PIN_1);

    driver1.begin();
    driver1.toff(5);          // Enables Driver in Software
    driver1.rms_current(MOTOR_CURRENT_PERCENT * 10);
    driver1.microsteps(MOTOR_MICROSTEP);

    // driver.pwm_autoscale(true);    // Needed for stealthChop
    // driver.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
    driver1.en_spreadCycle(false);

    stepper1.setCurrentPositionInSteps(0);
    stepper1.setSpeedInStepsPerSecond(400);
    stepper1.setAccelerationInStepsPerSecondPerSecond(400);
  #endif

  #ifdef MOTOR_2
    pinMode(STEP_PIN_2, OUTPUT);
    pinMode(DIR_PIN_2, OUTPUT);
    pinMode(EN_PIN_2, OUTPUT);
    digitalWrite(EN_PIN_2, HIGH);  // Hardware Enable

    stepper2.connectToPins(STEP_PIN_2, DIR_PIN_2);

    driver2.begin();
    driver2.toff(5);          // Enables Driver in Software
    driver2.rms_current(MOTOR_CURRENT_PERCENT * 10);
    driver2.microsteps(MOTOR_MICROSTEP);

    // driver.pwm_autoscale(true);    // Needed for stealthChop
    // driver.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle
    driver2.en_spreadCycle(false);

    stepper2.setCurrentPositionInSteps(0);
    stepper2.setSpeedInStepsPerSecond(400);
    stepper2.setAccelerationInStepsPerSecondPerSecond(400);
  #endif

  #ifdef MOTOR_3
    // motor3.setRunCurrent(MOTOR_CURRENT);
    // motor3.microsteps(MOTOR_MICROSTEP);
    // motor3.enableCoolStep();
    // motor3.enable();
  #endif
}


/**
 * Function to Test Angle Seek Feature for Motor
 * param[in] angle_delta : Angle to Rotate
 * param[in] direction   : Clockwise (1) or Anti-Clockwise (-1) Rotation
 */
void stepper1_test(long angle_delta) {
  // Calculate the target position based on the current position and desired angle
  long currentPos = stepper1.getCurrentPositionInSteps();
  long targetPos = currentPos + angle_delta * STEPS_PER_REVOLUTION / 360;

  // Move to the target position
  stepper1.moveToPositionInSteps(targetPos);
}

void stepper2_test(long angle_delta) {
  // Calculate the target position based on the current position and desired angle
  long currentPos = stepper2.getCurrentPositionInSteps();
  long targetPos = currentPos + angle_delta * STEPS_PER_REVOLUTION / 360; // Subtract for counterclockwise rotation

  // Move to the target position
  stepper2.moveToPositionInSteps(targetPos);
}


void setup() {
  // Initialize Stepper Motors
  init_steppers();
  // Set the button pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    // Rotate motor 1 by 30 degrees clockwise
    stepper1_test(30);

    // Rotate motor 2 by 30 degrees counterclockwise
    // stepper2_test(30);

    // Delay for stability
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void motorReverse(){
  //MOVE MOTOR VIA UART AND CHANGE DIRECTION VIA SOFTWARE, IT RUNS AS LONG AS YOU LET IT... PROBABLY ONLY USEFUL WITH ENCODER. THE VALUE SETS ONLY THE SPEED.

  driver1.VACTUAL(16000); //SET SPEED OF MOTOR
  delay(2000); // MOTOR MOVES 2 SEC THEN STOPS
  driver1.VACTUAL(0); //STOP MOTOR BY SETTING SPEED TO 0
  shatf1 = !shatf1; // REVERSE DIRECTION
  driver1.shaft(shatf1); // SET DIRECTION
}

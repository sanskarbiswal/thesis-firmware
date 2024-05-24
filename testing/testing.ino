#include <SpeedyStepper.h>
// #include <SoftwareSerial.h>
// #include <TMCStepper.h>

const int STEP = 3;
const int DIR = 2;
const int M_EN = 19;
const int M1_TX = 4;
const int M1_RX = 5;

const int STEP2 = 6;
const int DIR2 = 7;
const int M_EN2 = 20;

// #define DRV_ADDR 0b00
// #define R_SENSE  0.11f

const int steps_per_rev = 200;

// SoftwareSerial m1_uart(M1_RX, M1_TX);
// TMC2209Stepper driver(M1_RX, M1_TX, R_SENSE, DRV_ADDR);
SpeedyStepper stepper;
SpeedyStepper stepper2;

const int valve = 22;
// // #define valve 22


void setup() {
//   // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(M_EN, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(M_EN2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(STEP2, OUTPUT);
  // Serial.begin(9600);
  // m1_uart.begin(38400);

  stepper.connectToPins(STEP, DIR);
  stepper.setSpeedInStepsPerSecond(100);
  stepper.setAccelerationInStepsPerSecondPerSecond(100);
  digitalWrite(M_EN, HIGH);

  stepper2.connectToPins(STEP2, DIR2);
  stepper2.setSpeedInStepsPerSecond(100);
  stepper2.setAccelerationInStepsPerSecondPerSecond(100);
  digitalWrite(M_EN2, HIGH);

  // driver.begin();
  // driver.toff(5);
  // driver.rms_current(500);
  // driver.pwm_autoscale(true);    // Needed for stealthChop

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(valve, HIGH);
  digitalWrite(M_EN, LOW);
  stepper.moveRelativeInSteps(1000);
  digitalWrite(M_EN, HIGH);
  digitalWrite(M_EN2, LOW);
  stepper2.moveRelativeInSteps(1000);
  digitalWrite(M_EN2, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  // digitalWrite(valve, LOW);
  digitalWrite(M_EN, LOW);
  stepper.moveRelativeInSteps(-1000);
  digitalWrite(M_EN, HIGH);
  digitalWrite(M_EN2, LOW);
  stepper2.moveRelativeInSteps(-1000);
  digitalWrite(M_EN2, HIGH);
  delay(500);

}

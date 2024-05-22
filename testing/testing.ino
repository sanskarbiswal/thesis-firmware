#include <SpeedyStepper.h>
#include <SoftwareSerial.h>
#include <TMCStepper.h>

const int STEP = 9;
const int DIR = 10;
const int M_EN = 20;
const int M1_TX = 8;
const int M1_RX = 9;

#define DRV_ADDR 0b00
#define R_SENSE  0.11f

const int steps_per_rev = 200;

SoftwareSerial m1_uart(M1_RX, M1_TX);
TMC2209Stepper driver(M1_RX, M1_TX, R_SENSE, DRV_ADDR);
SpeedyStepper stepper;


const int valve = 22;
// #define valve 22


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(M_EN, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  Serial.begin(9600);
  m1_uart.begin(38400);

  stepper.connectToPins(STEP, DIR);
  stepper.setSpeedInStepsPerSecond(100);
  stepper.setAccelerationInStepsPerSecondPerSecond(100);
  digitalWrite(M_EN, LOW);

  driver.begin();
  driver.toff(5);
  driver.rms_current(1000);
  driver.pwm_autoscale(true);    // Needed for stealthChop

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(valve, HIGH);
  stepper.moveRelativeInSteps(1000);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(valve, LOW);
  delay(500);

}

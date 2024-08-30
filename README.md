
# PnPController_v2

## Overview
The `PnPController_v2` is an Arduino-based controller designed for a pick-and-place machine. It manages stepper motors, solenoid valves, and interfaces with a Raspberry Pi for control commands and status reporting. The system is primarily used for precise control of a Z-axis rig and an air valve, typical in automation and pick-and-place tasks.

## Hardware Connections

### GPIO Connections
- **Raspberry Pi UART Connection**
  - `PI_TX (12)`: UART TX pin for communication with Raspberry Pi.
  - `PI_RX (13)`: UART RX pin for communication with Raspberry Pi.
  
- **Motor 1**
  - `M1_DIR (2)`: Direction control pin.
  - `M1_STEP (3)`: Step control pin.
  - `M1_EN (19)`: Enable pin (active low).

- **Motor 2**
  - `M2_DIR (7)`: Direction control pin.
  - `M2_STEP (6)`: Step control pin.
  - `M2_EN (20)`: Enable pin (active low).

- **Solenoid Valve**
  - `VALVE_CTLR (22)`: Control pin for the solenoid valve.

- **Control Pins from Raspberry Pi**
  - `S0_PIN (26)`: Toggle valve state.
  - `S1_PIN (27)`: Toggle Z-rig position.
  - `S2_PIN (28)`: Additional control input (purpose not explicitly stated).
  - `S_INT (21)`: Interrupt notification pin.

## Software Components

### Libraries
- **SpeedyStepper**: Used for controlling the stepper motors with precise positioning.

### Definitions
- `STEPS_PER_REV (200)`: Number of steps per revolution for the stepper motors.
- `R_SENSE (0.11f)`: Current sensing resistor value.

### Functions

#### Initialization
- **`void init_gpio(void)`**  
  Initializes all GPIO pins used in the project. Sets up pin modes for motor control, solenoid valve control, and input pins connected to the Raspberry Pi.

- **`void init_stepper_motors(void)`**  
  Initializes the stepper motors, setting up the steps per revolution and connecting the appropriate pins.

- **`void init_intr(void)`**  
  Initializes the interrupt mechanism, setting up interrupt service routines (ISRs) to handle external interrupts triggered by the Raspberry Pi.

#### Motor Control
- **`void enable_motor(int motor)`**  
  Enables the specified motor (1 or 2) by pulling the enable pin low.

- **`void disable_motor(int motor)`**  
  Disables the specified motor (1 or 2) by pulling the enable pin high.

#### Valve Control
- **`void valve_ON(void)`**  
  Activates the solenoid valve by setting the control pin high.

- **`void valve_OFF(void)`**  
  Deactivates the solenoid valve by setting the control pin low.

- **`void toggle_valve(void)`**  
  Toggles the state of the solenoid valve between ON and OFF.

#### Z-Rig Control
- **`void toggle_z_rig(void)`**  
  Toggles the position of the Z-axis rig, moving it up or down based on the current state.

#### UART Communication
- **`void handle_uart(String command)`**  
  Handles incoming UART commands from the Raspberry Pi, executing actions such as turning the valve on/off or toggling the Z-rig position.

- **`void transmitStatus(void)`**  
  Transmits the current status of the tool (valve and rig states) back to the Raspberry Pi via UART.

#### Utility
- **`void resumePrinterOps(void)`**  
  Signals the printer to resume its operations after the pick-and-place process is completed.

#### Testing
- **`void test_motor_2(void)`**  
  A diagnostic function to manually test Motor 2 by moving it forward and backward.

## Usage
1. **Setup**: Connect the hardware components as specified in the GPIO connections section.
2. **Initialization**: Call `init_gpio()`, `init_stepper_motors()`, and `init_intr()` during the setup phase to initialize all the hardware.
3. **Control**: Use the available functions to control the motors and valve, or handle commands from the Raspberry Pi via UART.

## Notes
- Ensure the Raspberry Pi is properly connected and configured to send and receive UART commands.
- The functions provided allow for precise control over the pick-and-place mechanism, making it suitable for automation tasks requiring high accuracy.

## License
This project is open-source, and you are free to use and modify it according to your needs.

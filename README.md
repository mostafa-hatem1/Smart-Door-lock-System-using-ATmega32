# Dual Microcontroller-Based Door Locker Security System Using Password Authentication

## Overview

This project implements a smart door control system utilizing two microcontrollers: the Human-Machine Interface ECU (HMI_ECU) and the Control ECU (Control_ECU). These two units communicate via UART to provide a secure and efficient door locking mechanism with password authentication.

The system allows users to interact through an LCD and keypad to enter and manage passwords, which are securely stored in an external EEPROM. The door's locking mechanism is controlled by an H-bridge connected to a motor. Additionally, the system incorporates a PIR sensor for motion detection and a buzzer for alarms, enhancing its overall functionality. This project also feautures dynamically configurable drivers for maximum portability and reuasability

## Features

1.  **Password Protection:** Users can set and verify a password that is stored in external EEPROM.
2.  **LCD and Keypad Interface:** Provides a user-friendly interface for password entry and system management.
3.  **UART Communication:** Enables seamless data exchange between the HMI_ECU and the Control_ECU.
4.  **EEPROM Storage:** Securely stores passwords and other system data in external EEPROM.
5.  **H-Bridge Motor Control:** Controls the door locking/unlocking mechanism using a motor driven by an H-bridge.
6.  **PIR Sensor Integration:** Detects motion near the door, potentially triggering actions like holding the door open.
7.  **Buzzer for Alarms:** Provides auditory feedback for events or security alerts.
8.  **Password-Protected Access:** Limits access to system options like door unlocking and password changes to authorized users.

## Hardware Components


* HMI_ECU (Microcontroller)
* Control_ECU (Microcontroller)
* LCD
* Keypad
* External EEPROM (connected via I2C to Control_ECU)
* H-Bridge
* DC Motor (connected to Control_ECU, controlled by Timer0 PWM)
* PIR Sensor (connected to Control_ECU)
* Buzzer (connected to Control_ECU)

## Software Components

* **HMI Driver:** For interfacing with the LCD and Keypad.
* **Keypad Driver:** To handle user input from the keypad.
* **LCD Driver:** To display information to the user.
* **UART Driver:** For serial communication between the two ECUs.
* **EEPROM Driver:** To read and write data to the external EEPROM (using I2C).
* **DC Motor Driver:** To control the door lock motor (using Timer0 PWM).
* **Timer Driver:** Configured for PWM generation for motor control.
* **PIR Driver:** To detect motion using the PIR sensor.
* **Buzzer Driver:** To generate sound for alarms or feedback.

## Drivers Overview

This section provides a brief description of each driver used in the project, highlighting its purpose and key functionality.

### 1. GPIO Driver
- Handles general-purpose input/output pins for both HMI_ECU and Control_ECU.

### 2. UART Driver
- Manages UART communication between HMI_ECU and Control_ECU.
- Modified to accept a configuration structure (`UART_ConfigType`) for flexible parameter setup.

### 3. LCD Driver
- Controls a 2x16 LCD in 8-bit data mode or 4-bit data mode.
- Used in the HMI_ECU for displaying messages and prompts.

### 4. Keypad Driver
- Interfaces with a 4x4 keypad connected to the HMI_ECU.
- Processes user input for password entry and system navigation.

### 5. I2C Driver
- Facilitates communication with external EEPROM using the I2C protocol.
- Modified to accept a configuration structure (`TWI_ConfigType`) for dynamic configuration.

### 6. PWM Driver
- Generates PWM signals using Timer0.
- Used to control the motor at maximum speed through the H-bridge circuit.

### 7. Timer Driver
- Provides timing functionality using Timer0, Timer1, and Timer2 with interrupt-based callbacks.
- Supports both normal and compare modes for various timing tasks in HMI_ECU and Control_ECU.

### 8. Buzzer Driver
- Activates the buzzer for system alerts, such as failed password attempts.
- Connected to the Control_ECU.

### 9. PIR Sensor Driver
- Detects motion near the door via a PIR sensor.
- Includes functions for initialization (`PIR_init`) and state retrieval (`PIR_getState`).

### 10. DC Motor Driver
- Controls the motor for door operations using Timer0 PWM.
- Operates at maximum speed and interacts with the H-bridge circuit.

### 11. EEPROM Driver
- Manages data storage and retrieval in external EEPROM via I2C.
- Ensures secure storage of passwords and system configuration.

## Video References


* Video Reference:https://elsewedyedu1-my.sharepoint.com/:v:/g/personal/mh2200200_tkh_edu_eg/EeBebbgSbApIiR4kIaIW-0QBoeAwBFt0ytd4e7wjfE-msg?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=FLYuIa


## Acknowledgements

Special thanks to Edges For Training Team for their guidance and resources.



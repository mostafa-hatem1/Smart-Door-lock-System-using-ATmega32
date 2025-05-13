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

## Driver Requirements

* **Buzzer Driver:** Reuses the driver from previous projects, connected to the Control_ECU.
* **PIR Driver:**
    * Initializes the PIR sensor (`PIR_init()`).
    * Gets the PIR sensor state (`PIR_getState()`), returns `uint8`.
    * Connected to the Control_ECU. When motion is detected, it keeps the door held open.
* **DC Motor Driver:**
    * Reuses the driver from the Smart Home Project.
    * Motor runs at maximum speed using Timer0 PWM.
    * Connected to the Control_ECU.
* **EEPROM Driver:**
    * Reuses the external EEPROM driver controlled by I2C.
    * Connected to the Control_ECU.
* **Timer Driver:**
    * Configurable clock source (`timer_clock`).
    * Configurable timer mode (`timer_mode`).

## Video References


* Video Reference:https://elsewedyedu1-my.sharepoint.com/:v:/g/personal/mh2200200_tkh_edu_eg/EeBebbgSbApIiR4kIaIW-0QBoeAwBFt0ytd4e7wjfE-msg?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=FLYuIa


## Acknowledgements

Special thanks to Edges For Training Team for their guidance and resources.



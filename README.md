# Dual Microcontroller-Based Door Locker Security System Using Password Authentication

## Overview

This project implements a smart door control system utilizing two microcontrollers: the Human-Machine Interface ECU (HMI_ECU) and the Control ECU (Control_ECU). These two units communicate via UART to provide a secure and efficient door locking mechanism with password authentication.

The system allows users to interact through an LCD and keypad to enter and manage passwords, which are securely stored in an external EEPROM. The door's locking mechanism is controlled by an H-bridge connected to a motor. Additionally, the system incorporates a PIR sensor for motion detection and a buzzer for alarms, enhancing its overall functionality.

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

*(Based on the requirements mentioned in the document)*

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

*(Based on the requirements mentioned in the document)*

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

*(Summarized from the document)*

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

* How to add PIR sensor library to Proteus: [https://youtu.be/bhT90bM-Vdw](https://youtu.be/bhT90bM-Vdw)
* Video Reference: [https://youtu.be/X5EwIRfGAAY](https://youtu.be/X5EwIRfGAAY)
* PIR sensor library download link: [https://www.mediafire.com/file/vjffcf868ea2fbi/PIR_Sensor_lib.zip/file](https://www.mediafire.com/file/vjffcf868ea2fbi/PIR_Sensor_lib.zip/file)

## Thank You

Edges For Training Team

/*
 * CONTROL_ECU.c
 * Integrated with EEPROM, motor, buzzer, and PIR
 */
#include "std_types.h"
#include "uart.h"
#include "external_eeprom.h"
#include "Motor.h"
#include "buzzer.h"
#include "twi.h"
#include "PIR.h"
#include "timer.h"
#include "interrupt.h"
#include <util/delay.h>

/*---- System Configuration Constants ----*/
#define PASSWORD_LENGTH      5
#define EEPROM_BASE_ADDRESS  0x0311
#define MAX_ATTEMPTS         3
#define LOCKING_TIME         1
#define LOCKOUT_TIME         3
#define EEPROM_INIT_FLAG     0x55
#define EEPROM_FLAG_ADDR     (EEPROM_BASE_ADDRESS + PASSWORD_LENGTH)

/*---- UART Command Definitions ----*/
typedef enum {
	CMD_CREATE_PASSWORD = 0x01,
	CMD_OPEN_DOOR       = 0x03,
	CMD_CHANGE_PASSWORD = 0x04,
	CMD_LOCK_SYSTEM     = 0x06,
	CMD_CHECK_INIT      = 0x07
} UART_Command;

/*---- UART Response Definitions ----*/
typedef enum {
	RESPONSE_OK           = 0xAA,
	RESPONSE_ERROR        = 0xFF,
	RESPONSE_PIR_DETECTED = 0x55,
	RESPONSE_PIR_NOT_DETECTED = 0x66,
	NEXT_DIGIT = 0xE3,
} UART_Response;

/*---- Peripheral Configuration Structures ----*/
TWI_ConfigType twi_config = {
		.address = 0x01,     /*---- Optional I2C slave address ----*/
		.bit_rate = 0x02     /*---- 400kHz SCL for 8MHz CPU ----*/
};

UART_ConfigType uart_config = {
		.bit_data = UART_8_BIT_DATA,
		.parity = UART_PARITY_DISABLED,
		.stop_bit = UART_1_STOP_BIT,
		.baud_rate = 9600
};

/*---- Timer Control Variables ----*/
static volatile uint8 timer_elapsed = 0;  /*---- Global timer counter ----*/

/*---- Timer Callback Function ----*/
void Timer_CTC_Callback(void) {
	timer_elapsed++;
}

/*---- Non-blocking Delay Function ----*/
void Control_delaySeconds(uint8 seconds) {
	Timer_ConfigType timerConfig = {
			.initial_value = 0,
			.compare_value = 31249,    /*---- 1 second at 8MHz/256 prescaler ----*/
			.timer_id = TIMER1_ID,
			.mode = TIMER_MODE_CTC,
			.prescaler = TIMER_PRESCALER_256
	};

	timer_elapsed = 0;
	Timer_init(&timerConfig);
	Timer_setCallBack_CTC(Timer_CTC_Callback, TIMER1_ID);

	while (timer_elapsed < seconds);  /*---- Wait for specified time ----*/
	Timer_deInit(TIMER1_ID);
}

/*---- Check if Password is Stored in EEPROM ----*/
uint8 IsPasswordStored() {
	uint8 flag;
	EEPROM_readByte(EEPROM_FLAG_ADDR, &flag);
	_delay_ms(10); /*---- EEPROM read delay ----*/
	return (flag == EEPROM_INIT_FLAG);
}

/*---- Receive Password with Handshaking ----*/
void receivePassword(uint8* buffer) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		buffer[i] = UART_recieveByte();
		UART_sendByte(NEXT_DIGIT); /*---- Acknowledge each digit ----*/
	}
}

/*---- Compare Two Passwords ----*/
uint8 comparePasswords(uint8* p1, uint8* p2) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		if (p1[i] != p2[i]) return 0;
	}
	return 1;
}

/*---- Save Password to EEPROM ----*/
void savePasswordToEEPROM(uint8* password) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_writeByte(EEPROM_BASE_ADDRESS + i, password[i]);
		_delay_ms(10); /*---- EEPROM write delay ----*/
	}
	EEPROM_writeByte(EEPROM_FLAG_ADDR, EEPROM_INIT_FLAG); /*---- Set initialization flag ----*/
	_delay_ms(10);
}

/*---- Read Password from EEPROM ----*/
void readPasswordFromEEPROM(uint8* password) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_readByte(EEPROM_BASE_ADDRESS + i, &password[i]);
		_delay_ms(10); /*---- EEPROM read delay ----*/
	}
}

/*---- Door Control Sequence ----*/
void Door_Control_Sequence(void) {
	/*---- Phase 1: Unlocking ----*/
	Motor_rotate(MOTOR_CW, 100); /*---- Rotate motor clockwise ----*/
	Control_delaySeconds(LOCKING_TIME);
	Motor_rotate(MOTOR_STOP, 0);

	/*---- Check for motion detection ----*/
	if(PIR_getState() == LOGIC_HIGH) {
		UART_sendByte(RESPONSE_PIR_DETECTED);
		while (PIR_getState() == LOGIC_HIGH){} /*---- Wait while motion detected ----*/
	}
	UART_sendByte(RESPONSE_PIR_NOT_DETECTED);

	/*---- Phase 2: Locking ----*/
	Motor_rotate(MOTOR_ACW, 100); /*---- Rotate motor counter-clockwise ----*/
	Control_delaySeconds(LOCKING_TIME);

	/*---- Final Phase: Stop Motor ----*/
	Motor_rotate(MOTOR_STOP, 0);
}

/*---- Global Variables ----*/
uint8 failedAttempts = 0; /*---- Track failed password attempts ----*/
uint8 command; /*---- Current UART command ----*/

/*---- Main Application Entry Point ----*/
int main() {
	/*---- Initialize Peripherals ----*/
	UART_init(&uart_config);
	Motor_init();
	Buzzer_init();
	PIR_init();
	TWI_init(&twi_config);
	Enable_Global_Interrupt();

	/*---- Password Storage Variables ----*/
	uint8 storedPassword[PASSWORD_LENGTH], receivedPassword[PASSWORD_LENGTH], EEPROMPassword[PASSWORD_LENGTH];

	/*---- Main Command Processing Loop ----*/
	while (1) {
		command = UART_recieveByte();

		switch (command) {
		/*---- Check Initialization Status ----*/
		case CMD_CHECK_INIT:
			UART_sendByte(IsPasswordStored());
			break;

			/*---- Password Creation Command ----*/
		case CMD_CREATE_PASSWORD:
			receivePassword(receivedPassword);
			receivePassword(storedPassword); /*---- Reuse buffer for confirmation ----*/

			if (comparePasswords(receivedPassword, storedPassword)) {
				savePasswordToEEPROM(receivedPassword);
				UART_sendByte(RESPONSE_OK);
			} else {
				UART_sendByte(RESPONSE_ERROR);
			}
			break;

			/*---- Door Unlock Command ----*/
		case CMD_OPEN_DOOR:
			receivePassword(receivedPassword);
			readPasswordFromEEPROM(EEPROMPassword);

			if (comparePasswords(receivedPassword, EEPROMPassword)) {
				UART_sendByte(RESPONSE_OK);
				Door_Control_Sequence();
			} else {
				UART_sendByte(RESPONSE_ERROR);
				failedAttempts++;
			}
			break;

			/*---- System Lockout Command ----*/
		case CMD_LOCK_SYSTEM:
			Buzzer_on();
			Control_delaySeconds(LOCKOUT_TIME);
			Buzzer_off();
			failedAttempts = 0; /*---- Reset attempt counter ----*/
			break;

			/*---- Password Change Command ----*/
		case CMD_CHANGE_PASSWORD:
			receivePassword(receivedPassword);
			readPasswordFromEEPROM(EEPROMPassword);

			if (comparePasswords(receivedPassword, EEPROMPassword)) {
				UART_sendByte(RESPONSE_OK);
				receivePassword(receivedPassword);
				receivePassword(storedPassword);

				if (comparePasswords(receivedPassword, storedPassword)) {
					savePasswordToEEPROM(receivedPassword);
					UART_sendByte(RESPONSE_OK);
				} else {
					UART_sendByte(RESPONSE_ERROR);
				}
			} else {
				UART_sendByte(RESPONSE_ERROR);
				failedAttempts++;
			}
			break;
		}
	}
}

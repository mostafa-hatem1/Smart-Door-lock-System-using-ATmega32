/*
 * HMI_App.c
 * Integrated version with timer, UART handshaking, and door control
 */
#include "std_types.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include "interrupt.h"
#include <util/delay.h>

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

/*---- System Constants ----*/
#define PASSWORD_LENGTH  5
#define MAX_ATTEMPTS     3
#define ENTER_KEY        ENTER
#define LOCKING_TIME     1
#define LOCKOUT_TIME     3

/*---- System State Definitions ----*/
typedef enum {
	STATE_CREATE_PASSWORD,
	STATE_MAIN_OPTIONS,
	STATE_OPEN_DOOR,
	STATE_CHANGE_PASSWORD,
	STATE_LOCKED,
} SystemState;

/*---- Global Variables ----*/
UART_ConfigType uart_config = {
		.bit_data = UART_8_BIT_DATA,
		.parity = UART_PARITY_DISABLED,
		.stop_bit = UART_1_STOP_BIT,
		.baud_rate = 9600
};

static volatile uint8 time_elapsed = 0;
uint8 failedAttempts = 0;
uint8 systemInitialized = 0;
uint8 password[PASSWORD_LENGTH], confirmPassword[PASSWORD_LENGTH];
uint8 error;
uint8 response;

/*---- Timer Callback Function ----*/
void Timer_CTC_Callback(void) {
	time_elapsed++;
}

/*---- Non-blocking Delay Function ----*/
void HMI_delaySeconds(uint8 seconds) {
	Timer_ConfigType timerConfig = {
			.initial_value = 0,
			.compare_value = 31249,    /*---- 8MHz/256 prescaler = 1 second ----*/
			.timer_id = TIMER1_ID,
			.mode = TIMER_MODE_CTC,
			.prescaler = TIMER_PRESCALER_256
	};

	time_elapsed = 0;
	Timer_init(&timerConfig);
	Timer_setCallBack_CTC(Timer_CTC_Callback, TIMER1_ID);

	while (time_elapsed < seconds); /*---- Wait for specified time ----*/

	Timer_deInit(TIMER1_ID);       /*---- Cleanup timer ----*/
}

/*---- Send Password with Handshaking ----*/
void sendPassword(uint8* password) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(password[i]);
		while (UART_recieveByte() != NEXT_DIGIT); /*---- Wait for acknowledgment ----*/
	}
}

/*---- Get Password from Keypad ----*/
void getPassword(uint8* buffer) {
	uint8 key, count = 0;
	while (count < PASSWORD_LENGTH) {
		key = KEYPAD_getPressedKey();
		if (key >= 0 && key <= 9) {
			buffer[count] = key;
			LCD_moveCursor(1,count);
			LCD_displayCharacter('*'); /*---- Mask password input ----*/
			count++;
		}
		_delay_ms(350); /*---- Keypad debounce delay ----*/
	}
	while (KEYPAD_getPressedKey() != ENTER_KEY); /*---- Wait for ENTER confirmation ----*/
}

/*---- Door Unlock Sequence ----*/
void unlockDoor() {
	UART_sendByte(CMD_OPEN_DOOR);
	uint8 response;
	error = 0;
	LCD_clearScreen();
	LCD_displayString("Enter pass: ");
	getPassword(password);
	sendPassword(password);
	response = UART_recieveByte();

	if (response == RESPONSE_OK){
		/*---- Unlocking procedure ----*/
		error = 0;
		LCD_clearScreen();
		LCD_displayString("UNLOCKING...");
		HMI_delaySeconds(LOCKING_TIME);

		/*---- Wait for PIR sensor clearance ----*/
		while (UART_recieveByte() == RESPONSE_PIR_DETECTED){
			LCD_clearScreen();
			LCD_displayString("People entering");
		}

		/*---- Locking procedure ----*/
		LCD_clearScreen();
		LCD_displayString("LOCKING...");
		HMI_delaySeconds(LOCKING_TIME);
	} else if(response == RESPONSE_ERROR){
		/*---- Password error handling ----*/
		LCD_clearScreen();
		LCD_displayString("wrong pass");
		HMI_delaySeconds(1);
		failedAttempts++;
		error = 1;
	}
}

/*---- Main Application Entry Point ----*/
int main() {
	/*---- Initialize peripherals ----*/
	LCD_init();
	UART_init(&uart_config);
	Enable_Global_Interrupt();

	/*---- Check if password exists in EEPROM ----*/
	UART_sendByte(CMD_CHECK_INIT);
	SystemState currentState = UART_recieveByte() ? STATE_MAIN_OPTIONS : STATE_CREATE_PASSWORD;

	if(currentState == STATE_MAIN_OPTIONS) {
		LCD_clearScreen();
		LCD_displayString("System Ready");
		HMI_delaySeconds(1);
	}

	/*---- Main State Machine ----*/
	while (1) {
		switch (currentState) {
		/*---- Password Creation State ----*/
		case STATE_CREATE_PASSWORD:
			LCD_clearScreen();
			LCD_displayString("Enter Password:");
			getPassword(password);

			LCD_clearScreen();
			LCD_displayString("Confirm Password:");
			getPassword(confirmPassword);

			/*---- Send passwords to Control ECU ----*/
			UART_sendByte(CMD_CREATE_PASSWORD);
			sendPassword(password);
			sendPassword(confirmPassword);

			if (UART_recieveByte() == RESPONSE_OK) {
				currentState = STATE_MAIN_OPTIONS;
			} else {
				LCD_clearScreen();
				LCD_displayString("Mismatch! Retry");
				HMI_delaySeconds(2);
			}
			break;

			/*---- Main Menu State ----*/
		case STATE_MAIN_OPTIONS:
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_displayStringRowColumn(1,0,"- : Change Pass");
			uint8 key = KEYPAD_getPressedKey();
			if (key == '+') {
				currentState = STATE_OPEN_DOOR;
			} else if (key == '-') {
				currentState = STATE_CHANGE_PASSWORD;
			}
			break;

			/*---- Door Unlock State ----*/
		case STATE_OPEN_DOOR:
			unlockDoor();
			if(error){
				if(failedAttempts == MAX_ATTEMPTS){
					currentState = STATE_LOCKED;
				} else {
					unlockDoor(); /*---- Retry on failed attempt ----*/
				}
			} else {
				currentState = STATE_MAIN_OPTIONS;
			}
			break;

			/*---- System Lockout State ----*/
		case STATE_LOCKED:
			UART_sendByte(CMD_LOCK_SYSTEM);
			LCD_clearScreen();
			LCD_displayString("Locked(60s)");
			HMI_delaySeconds(LOCKOUT_TIME);
			failedAttempts = 0;
			currentState = STATE_MAIN_OPTIONS;
			break;

			/*---- Password Change State ----*/
		case STATE_CHANGE_PASSWORD:
			UART_sendByte(CMD_CHANGE_PASSWORD);
			LCD_clearScreen();
			LCD_displayString("Enter old pass");
			getPassword(password);
			sendPassword(password);
			response = UART_recieveByte();

			if(response == RESPONSE_OK){
				/*---- New password entry ----*/
				LCD_clearScreen();
				LCD_displayString("Enter new pass");
				getPassword(password);

				LCD_clearScreen();
				LCD_displayString("Confirm new pass");
				getPassword(confirmPassword);

				sendPassword(password);
				sendPassword(confirmPassword);
				response = UART_recieveByte();

				if(response == RESPONSE_OK){
					LCD_clearScreen();
					LCD_displayString("New pass saved");
					HMI_delaySeconds(2);
					currentState = STATE_MAIN_OPTIONS;
				} else if(response == RESPONSE_ERROR){
					LCD_clearScreen();
					LCD_displayString("No match");
					HMI_delaySeconds(1);
				}
			} else if(response == RESPONSE_ERROR){
				failedAttempts++;
				LCD_clearScreen();
				LCD_displayString("Wrong pass");
				HMI_delaySeconds(1);
				if(failedAttempts == MAX_ATTEMPTS){
					currentState = STATE_LOCKED;
				}
			}
			break;
		}
	}
}

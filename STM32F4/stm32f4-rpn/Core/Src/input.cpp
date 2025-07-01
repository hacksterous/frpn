// Copyright (c) 2014-2022 Louis Rubet

#include "main.h"
#include "input.h"

#include <cstring>

extern UART_HandleTypeDef huart1; //declared in main.cpp
uint8_t UART1RxBuffer[1] = {0};

Input::Input(string& entry): status(InputStatus::kOk) {
	while (1) {
		HAL_UART_Receive (&huart1, UART1RxBuffer, 1, HAL_MAX_DELAY);

		if (*UART1RxBuffer == 0x03) { //keyCtrlC
			status = InputStatus::kCtrlc;
			return;
		} else if (*UART1RxBuffer == 0xa || *UART1RxBuffer == 0xd) { //keyCR/LF
			status = InputStatus::kOk;
			return;
		} else if (*UART1RxBuffer == 0x04) { //keyCtrlD
			status = InputStatus::kAbort;
			return;
		} else {
			//status = InputStatus::kError;
			entry += char(*UART1RxBuffer);// string entry
			status = InputStatus::kOk;
		}
	}
}


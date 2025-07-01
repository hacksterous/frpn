// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

//FIXME: time functions are not available
/// @brief time keyword implementation
///
void Program::RpnTime() {
	//Only for STM32F4
    char buffer[20] = "time is not impl.";
    stack_.push(new String(buffer));
}

/// @brief date keyword implementation
///
void Program::RpnDate() {
	//Only for STM32F4
    char buffer[20] = "time is not impl.";
    stack_.push(new String(buffer));
}

/// @brief ticks keyword implementation
///
void Program::RpnTicks() {
	//Only for STM32F4
    char buffer[20] = "tick is not impl.";
    stack_.push(new String(buffer));
}

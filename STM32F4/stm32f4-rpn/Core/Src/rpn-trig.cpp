// Copyright (c) 2014-2022 Louis Rubet

#include "program.h"

#define CONST_PI 3.14159265358979324D
/// @brief pi keyword implementation
///
void Program::RpnPi(void) {
    stack_.push_front(new Number(CONST_PI));
}

/// @brief d->r keyword implementation
///
void Program::RpnD2r(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) *= CONST_PI;
    stack_.value<Number>(0) /= 180;
}

/// @brief r->d keyword implementation
///
void Program::RpnR2d(void) {
    MIN_ARGUMENTS(1);
    ARG_MUST_BE_OF_TYPE(0, kNumber);
    stack_.value<Number>(0) /= CONST_PI;
    stack_.value<Number>(0) *= 180;
}

/// @brief sin keyword implementation
///
void Program::RpnSin(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = sin(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = sin(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief asin keyword implementation
///
void Program::RpnAsin(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = asin(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = asin(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief cos keyword implementation
///
void Program::RpnCos(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = cos(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = cos(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief acos keyword implementation
///
void Program::RpnAcos(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = acos(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = acos(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief tan keyword implementation
///
void Program::RpnTan(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = tan(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = tan(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

/// @brief atan keyword implementation
///
void Program::RpnAtan(void) {
    MIN_ARGUMENTS(1);
    if (stack_.type(0) == kNumber)
        stack_.value<Number>(0) = atan(stack_.value<Number>(0));
    else if (stack_.type(0) == kComplex)
        stack_.value<Complex>(0) = atan(stack_.value<Complex>(0));
    else
        ERROR_CONTEXT(kBadOperandType);
}

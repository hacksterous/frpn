// Copyright (c) 2014-2022 Louis Rubet

#include "object.h"
#include "program.h"

// number statics
Number::mode_enum Number::mode = Number::kDefaultMode;
int Number::digits = kDefaultDecimalDigits;

ostream& Number::ShowValue(ostream& out, const long double& value, mode_enum mode, int digits, int base) {
    stringstream format;
    switch (base) {
        case 10:
            switch (mode) {
                case kStd:
                    format << "%." << digits << "Lg";
                    break;
                case kFix:
                    format << "%." << digits << "Lf";
                    break;
                case kSci:
                    format << "%." << digits << "Le";
                    break;
            }
            return RealFmtOutput(out, format.str(), value);
        case 16:
            if (mode == kStd) {
                return RealOutputNBase(out, base, value);  // custom function, as none available in mpfr
            } else {
                format << "%." << digits << "Lf";  // C99 format
                return RealFmtOutput(out, format.str(), value);
            }
        case 2:
            if (mode == kStd) {
                return RealOutputNBase(out, base, value);  // custom function, as none available in mpfr
            } else {
                format << "%." << digits << "Lf";  // C99 format
                return RealFmtOutput(out, format.str(), value);
            }
        default:
            // base other than 2, 10, 16
            return RealOutputNBase(out, base, value);
    }
}

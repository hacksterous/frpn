// Copyright (c) 2014-2022 Louis Rubet

#include "real-out.h"
#include "program.h"

ostream& RealFmtOutput(ostream& out, const string& fmt, const double& value) {
	char buffer[30];
	std::snprintf(buffer, sizeof(buffer), fmt.c_str(), value);
	//cout << "anirb: ReatOut::RealFmtOutput: " << string(buffer) << endl;
	out << string(buffer);
	return out;
}

static bool IsMin(const double& p) {
	bool ret;
	int round_away;
	int mode = std::fegetround();
	double frac;
	//FE_TONEAREST	Round to nearest (even) — default
	//FE_DOWNWARD	Round toward negative infinity (downward)
	//FE_UPWARD	Round toward positive infinity (upward)
	//FE_TOWARDZERO	Round toward zero (truncate fractional part)
	switch (mode) {
		case FE_TOWARDZERO:
			round_away = 0;
			break;
		case FE_DOWNWARD:
			round_away = (p < 0.0);
			break;
		case FE_UPWARD:
			round_away = (p > 0.0);
			break;
		case FE_TONEAREST:
			round_away = 0;
			frac = p - floorl(p);
			if (p > 0.0) {
				if (frac >= 0.5) round_away = 1;
			} else if (p < 0.0) {
				if (frac >= 0.5) round_away = 0;
			}
			break;
	}

	if (round_away) 
		/* round away from zero: the last output digit is '1' */
		ret = true;
	else
		/* only zeros in fractional part */
		ret = false;
	return ret;
}

static void OutBase(ostream& out, int base) {
	if (base == 16)
		out << "0x";
	else if (base == 2)
		out << "0b";
}

static ostream& OutSingular(ostream& out, int base, const double& value) {
	const char* write_after_sign = NULL;  // unused for now
	int digits = 0;					   // forced 0 digits after separator
	if (std::isnan(value)) {
		out << "nan";
	} else if (std::isinf(value)) {
		if (value < 0) out << '-';
		out << "inf";
	} else {
		// zero
		if (value < 0) out << '-';  // signed zero is allowed
		if (write_after_sign != NULL) out << write_after_sign;
		OutBase(out, base);
		out << '0';
		if (digits > 0) {
			out << '.';
			for (int i = 0; i < digits; i++) out << '0';
		}
	}
	return out;
}

static ostream& OutLittleNumber(ostream& out, int base, const double& value) {
	const char* write_after_sign = NULL;  // unused for now
	int digits = 0;					   // forced 0 digits after separator

	if (value < 0) out << '-';
	if (write_after_sign != NULL) out << write_after_sign;
	OutBase(out, base);
	out << '0';
	if (digits > 0) {
		out << '.';
		for (int i = 0; i < digits - 1; i++) out << '0';

		if (IsMin(value))
			out << '1';
		else
			out << '0';
	}
	return out;
}

static string hexFixStr(string hexSciStr) {
	char* end;
	double value;
	std::ostringstream oss;

	if (hexSciStr[0] == '-') {
		oss << '-';
		value = std::strtold(hexSciStr.substr(1).c_str(), &end);
	} else
		value = std::strtold(hexSciStr.c_str(), &end);

	long long intVal = static_cast<long long>(std::round(value));
	oss << "0x" << std::hex << intVal;
	return oss.str();
}

static string hexToBin(string hexStr) {
	std::string result;
	int bitPos = 0;

	for (char c: hexStr) {
		switch(std::toupper(c)) {
			case '-': result.replace(bitPos, 1, "-"); bitPos++; break;
			case '.': result.replace(bitPos, 1, "."); bitPos++; break;
			case '0': result.replace(bitPos, 4, "0000"); bitPos += 4; break;
			case '1': result.replace(bitPos, 4, "0001"); bitPos += 4; break;
			case '2': result.replace(bitPos, 4, "0010"); bitPos += 4; break;
			case '3': result.replace(bitPos, 4, "0011"); bitPos += 4; break;
			case '4': result.replace(bitPos, 4, "0100"); bitPos += 4; break;
			case '5': result.replace(bitPos, 4, "0101"); bitPos += 4; break;
			case '6': result.replace(bitPos, 4, "0110"); bitPos += 4; break;
			case '7': result.replace(bitPos, 4, "0111"); bitPos += 4; break;
			case '8': result.replace(bitPos, 4, "1000"); bitPos += 4; break;
			case '9': result.replace(bitPos, 4, "1001"); bitPos += 4; break;
			case 'a': result.replace(bitPos, 4, "1010"); bitPos += 4; break;
			case 'b': result.replace(bitPos, 4, "1011"); bitPos += 4; break;
			case 'c': result.replace(bitPos, 4, "1100"); bitPos += 4; break;
			case 'd': result.replace(bitPos, 4, "1101"); bitPos += 4; break;
			case 'e': result.replace(bitPos, 4, "1110"); bitPos += 4; break;
			case 'f': result.replace(bitPos, 4, "1111"); bitPos += 4; break;
		}
	}
	return result;
}

static ostream& OutNumber(ostream& out, int base, const double& value) {
	//if (base == 16) std::cout << "anirb: ReatOut::OutNumber -- in hex, r = " << std::hexfloat << value << std::endl;
   
	std::ostringstream temp;
	temp << value;
	//std::cout << "anirb: ReatOut::OutNumber -- r = " << value << " and base = " << base << std::endl;
	if (base == 2)
		out << hexToBin(hexFixStr(temp.str()));
	else if (base == 16)
		out << hexFixStr(temp.str());
	else 
		out << value;
	return out;
}

ostream& RealOutputNBase(ostream& out, int base, const double& value) {
	// singular = nan, +/-inf or zero
	//std::cout << "anirb: ReatOut::RealOutputNBase -- r = " << value << " and base = " << base << std::endl;
	if (value == 0.0 || std::isnan(value) || std::isinf(value)) return OutSingular(out, base, value);

	if (fabsl(value) < 1e-10) return OutLittleNumber(out, base, value);

	return OutNumber(out, base, value);
}

// Copyright (c) 2014-2022 Louis Rubet

#include <iostream>
#include <string>
#include <utility>
#include <cfenv>
using std::cout, std::string, std::pair;

#include "program.h"
#include "version.h"

#define ATTR_BOLD "\33[1m"
#define ATTR_OFF "\33[0m"

//FE_TONEAREST	Round to nearest (even) — default
//FE_DOWNWARD	Round toward negative infinity (downward)
//FE_UPWARD	Round toward positive infinity (upward)
//FE_TOWARDZERO	Round toward zero (truncate fractional part)

#define CFENV_ROUND \
	{"nearest (even)", FE_TONEAREST}, {"toward zero", FE_TOWARDZERO}, {"toward +inf", FE_UPWARD}, {"toward -inf", FE_DOWNWARD}

static const char _description[] =
	ATTR_BOLD "R" ATTR_OFF "everse " ATTR_BOLD "P" ATTR_OFF "olish " ATTR_BOLD "N" ATTR_OFF "otation CLI calculator";

static const char _syntax[] = ATTR_BOLD "Syntax" ATTR_OFF ": frpn [command]\nwith optional command = list of commands";

static const char _uname[] = ATTR_BOLD "frpn " FRPN_VERSION ", (c) 2022 <louis@rubet.fr>" ATTR_OFF;

static const char _welcome[] = ATTR_BOLD "frpn " FRPN_VERSION ATTR_OFF "\nType h or help for more information.";

/// @brief quit keyword implementation
///
void Program::RpnQuit() { ERROR_CONTEXT(kGoodbye); }

/// @brief nop keyword implementation
/// the result is written on stdout
///
void Program::RpnHelp() {
	// software name
	cout << endl << _uname << endl;

	// _description
	cout << _description << endl << endl;

	// _syntax
	cout << _syntax << endl;

	// keywords
	for (auto& kw : keywords_)
		if (!kw.comment.empty()) {
			// titles in bold
			if (kw.type == kUndef) cout << ATTR_BOLD;
			// show title or keyword + comment
			cout << kw.name << '\t' << kw.comment << endl;
			if (kw.type == kUndef) cout << ATTR_OFF;
		}
	cout << endl;

	// show mode
	cout << "Current float mode is ";
	switch (Number::mode) {
		case Number::kStd:
			cout << "'std'";
			break;
		case Number::kFix:
			cout << "'fix'";
			break;
		case Number::kSci:
			cout << "'sci'";
			break;
		default:
			cout << "unknown";
			break;
	}

	// bits precision, decimal digits and rounding mode
	cout << " with " << Number::digits << " digits after the decimal point" << endl;
	cout << "Current floating point precision is " << static_cast<int>(std::numeric_limits<double>::digits10) << " bits" << endl;
	vector<pair<string, int>> rnd{CFENV_ROUND};
	for (auto& rn : rnd)
		if (rn.second == std::fegetround()) {
			cout << "Current rounding mode is '" << rn.first << '\'' << endl;
			break;
		}
	cout << endl;
}

/// @brief welcome string
///
void Program::Welcome() { cout << _welcome << endl; }

/// @brief whether a printed precision is in the precision min/max
///
/// @param precision the precision in digits
/// @return true the precision is good
/// @return false the precision is not good
///
static bool check_decimal_digits(int precision) { return precision >= 0; }

/// @brief std keyword implementation
///
void Program::RpnStd() {
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, kNumber);

	int digits = static_cast<int>(stack_.value<Number>(0));

	if (check_decimal_digits(digits)) {
		// set mode, decimal digits and print format
		Number::mode = Number::kStd;
		Number::digits = digits;
		stack_.pop();
	} else {
		ERROR_CONTEXT(kOutOfRange);
	}
}

/// @brief fix keyword implementation
///
void Program::RpnFix() {
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, kNumber);

	int digits = static_cast<int>(stack_.value<Number>(0));

	if (check_decimal_digits(digits)) {
		// set mode, decimal digits and print format
		Number::mode = Number::kFix;
		Number::digits = digits;
		stack_.pop();
	} else {
		ERROR_CONTEXT(kOutOfRange);
	}
}

/// @brief sci keyword implementation
///
void Program::RpnSci() {
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, kNumber);

	int digits = static_cast<int>(stack_.value<Number>(0));

	if (check_decimal_digits(digits)) {
		// set mode, decimal digits and print format
		Number::mode = Number::kSci;
		Number::digits = digits;
		stack_.pop();
	} else {
		ERROR_CONTEXT(kOutOfRange);
	}
}

/// @brief _version keyword implementation
///
void Program::RpnVersion() { stack_.push_front(new String(FRPN_VERSION)); }

/// @brief _uname keyword implementation
///
void Program::RpnUname() { stack_.push_front(new String(_uname)); }

/// @brief history keyword implementation
///
void Program::RpnHistory() {
	// see command history on stdout
}

/// @brief type keyword implementation
///
void Program::RpnType() {
	MIN_ARGUMENTS(1);
	stack_.push(new String(stack_.at(0)->Name()));
	stack_.erase(1);
}

/// @brief default keyword implementation
///
void Program::RpnDefault() { Program::ApplyDefault(); }

/// @brief prec keyword implementation
///
void Program::RpnPrecision() {
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, kNumber);

	int prec = static_cast<int>(stack_.value<Number>(0));
	if (prec >= 1 && prec <= 15) {
		// modify digits seen by user if std mode
		if (Number::mode == Number::kStd) {
			// calc max nb of digits user can see with the current bit precision
			Number::digits = prec;
		}
		stack_.pop();
	} else {
		ERROR_CONTEXT(kOutOfRange);
	}
}

/// @brief round keyword implementation
///
void Program::RpnRound() {
	MIN_ARGUMENTS(1);
	ARG_MUST_BE_OF_TYPE(0, kString);

	map<string, int> matchRound{CFENV_ROUND};

	auto found = matchRound.find(stack_.value<String>(0));
	if (found != matchRound.end())
		std::fesetround(found->second);
	else
		ERROR_CONTEXT(kOutOfRange);
	stack_.pop();
}

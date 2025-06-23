// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_REAL_OUT_HPP_
#define SRC_REAL_OUT_HPP_

#include <string>
#include <ostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <cfenv>

using std::string;
using std::ostream;

ostream& RealFmtOutput(ostream& out, const string& fmt, const long double& value);
ostream& RealOutputNBase(ostream& out, int base, const long double& value);

#endif  // SRC_REAL_OUT_HPP_

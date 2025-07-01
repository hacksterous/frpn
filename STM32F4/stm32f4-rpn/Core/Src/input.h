// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_INPUT_H_
#define SRC_INPUT_H_

#include <cstdio>
#include <string>
#include <vector>
using std::string, std::vector;

class Input {
 public:
	Input(string& entry);
	enum class InputStatus {
		kOk,	 // user entered a good string
		kCtrlc,  // user pressed CtrlC
		kAbort,  // user aborted current line
		kError   // entry internal error
	} status;
};

#endif  // SRC_INPUT_H_

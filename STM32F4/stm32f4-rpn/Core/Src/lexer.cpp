// Copyright (c) 2014-2022 Louis Rubet

#include "lexer.h"
#include <iostream>

bool Lexer::Analyse(const string& entry, map<string, ReservedWord>& keywords, vector<SynElement>& elements,
					vector<SynError>& errors) {
	size_t jump;
	for (size_t i = 0; i < entry.size(); i++) {
		if (isspace(entry[i])) continue;
		SynElement element{.re = 0, .im = 0};
		switch (entry[i]) {
			case '"':
				if (!ParseString(entry, i, jump, errors, elements)) return false;
				i = jump - 1;
				continue;
			case '\'':
				if (!ParseSymbol(entry, i, jump, errors, elements)) return false;
				i = jump - 1;
				continue;
			case '(':
				if (!ParseComplex(entry, i, jump, errors, elements)) return false;
				i = jump - 1;
				continue;
		}
		if (i < entry.size() - 1 && (entry.substr(i, 2) == "<<" || entry.substr(i, 2) == "«")) {
			if (!ParseProgram(entry, i, jump, errors, elements)) return false;
			i = jump - 1;
			continue;
		} else if (ParseReserved(entry, i, jump, elements, keywords)) {
			// found a keywords word, add it with its correct type
			i = jump - 1;
			continue;
		} else if (ParseNumber(entry, i, jump, errors, elements)) {
			i = jump - 1;
			continue;
		}
		if (ParseUnknown(entry, i, jump, elements))
			// last chance, this unknown entry is treated as a symbol
			i = jump - 1;
		else
			return false;  // no object of any type could be found, this is a lexer error
	}

	return true;
}

void Lexer::Trim(string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

bool Lexer::ParseString(const string& entry, size_t idx, size_t& next_idx,
						vector<SynError>& errors __attribute__((unused)), vector<SynElement>& elements) {
	// here we are sure that entry[0] is at least '"'
	for (size_t i = idx + 1; i < entry.size(); i++) {
		if (entry[i] == '"') {
			if (entry[i] - 1 != '\\') {
				elements.push_back({.type = kString, .value = entry.substr(idx + 1, i - idx - 1)});
				next_idx = i + 1;
				return true;
			}
		}
	}
	elements.push_back({.type = kString, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
	next_idx = entry.size();
	return true;
}

bool Lexer::ParseSymbol(const string& entry, size_t idx, size_t& next_idx,
						vector<SynError>& errors __attribute__((unused)), vector<SynElement>& elements) {
	// here we are sure that entry[0] is at least '\''
	for (size_t i = idx + 1; i < entry.size(); i++) {
		if (entry[i] == '\'') {
			elements.push_back({.type = kSymbol, .value = entry.substr(idx + 1, i - idx - 1), .auto_eval = false});
			next_idx = i + 1;
			return true;
		}
	}
	elements.push_back({.type = kSymbol, .value = entry.substr(idx + 1, entry.size() - idx - 1)});
	next_idx = entry.size();
	return true;
}

bool Lexer::ParseProgram(const string& entry, size_t idx, size_t& next_idx,
						 vector<SynError>& errors __attribute__((unused)), vector<SynElement>& elements) {
	// here we are sure that entry is at least "<<"
	// find last ">>" or "»"
	int countNested = 0;
	for (size_t i = idx + 2; i < entry.size() - 1; i++) {
		if ((entry[i] == '<' && entry[i + 1] == '<') || (entry.substr(i, 2) == "«")) {
			countNested++;
		} else if ((entry[i] == '>' && entry[i + 1] == '>') || (entry.substr(i, 2) == "»")) {
			if (countNested == 0) {
				string prg = entry.substr(idx + 2, i - idx - 2);
				Trim(prg);
				elements.push_back({.type = kProgram, .value = prg});
				next_idx = i + 2;
				return true;
			} else {
				countNested--;
			}
		}
	}
	string prg = entry.substr(idx + 2, entry.size() - idx - 2);
	Trim(prg);
	elements.push_back({.type = kProgram, .value = prg});
	next_idx = entry.size();
	return true;
}

int Lexer::GetBaseAt(const string& entry, size_t& next_idx) {
	// a regex could be "([+-])?((0[xX])|([0-9][0-9]?[bB]))"
	// regex is not use because dramatically slow
	// entry is scanned from idxStart, searching for [s]abc (sign and 3 first chars)
	size_t scan = 0;
	next_idx = 0;
	if (scan >= entry.size()) return 10;
	if (entry[scan] == '+' || entry[scan] == '-') {
		scan++;
		next_idx = scan;
	}
	if (scan + 2 >= entry.size()) return 10;
	char a = entry[scan];
	char b = entry[scan + 1];
	if (a == '0') {
		if (b == 'b' || b == 'B') {
			next_idx = scan + 2;
			return 2;
		}
		if (b == 'd' || b == 'D') {
			next_idx = scan + 2;
			return 10;
		}
		if (b == 'x' || b == 'X') {
			next_idx = scan + 2;
			return 16;
		}
	} 
	if (a == 'b' || a == 'B') {
			next_idx = scan + 1;
			return 2;
	} 
	if (b == 'd' || b == 'D') {
		next_idx = scan + 2;
		return 10;
	}
	if (a == 'h' || a == 'H' || a == 'x' || a == 'X') {
		next_idx = scan + 1;
		return 16;
	}
	return 10;
}

bool Lexer::stringToDouble(const char* str, double* dbl, int base) {
	char* endPtr;
	char temp[100];
	double localDbl;
	//only base-2/10/16 are allowed
	//std::cout << "anirb: Lexer::stringToDouble -- initial str = " << str << "\r\n";

	int signPos = 0;
	if (str[0] == '+' || str[0] == '-') {
		//has a sign indicator
		temp[0] = str[0];
		signPos = 1;
	}
	if (str[signPos] == 'h' || str[signPos] == 'x' || str[signPos] == 'b') {
		//number is of the form -x100 = decimal -256, or
		//-b100 = decimal -4
		//modify the prefix to a standard format
		if (base == 2)
			strcpy(temp + signPos, "0b");
		else if (base == 16)
			strcpy(temp + signPos, "0x");
		strcpy(temp + 2 + signPos, str + signPos + 1); //copy the non-standard format number
	} else {
		strcpy(temp + signPos, str + signPos); //copy the standard format number
	}
	localDbl = std::strtod(temp, &endPtr);
	//std::cout << "anirb: Lexer::stringToDouble -- got final double = " << localDbl << "\r\n"; //<- Hangs microcontroller
	//std::cout << "anirb: Lexer::stringToDouble -- got final str = " << temp << "\r\n";
	//if (*endPtr != '\0') std::cout << "Got bad double\r\n";	
	if (*endPtr != '\0') return false; // bad number	
	*dbl = localDbl;
	return true; // good number
}

bool Lexer::GetNumberAt(const string& entry, size_t idx, size_t& next_idx, int& base, double* r, char delim) {
	stringstream ss;
	string token;
	int convstat;
	next_idx = idx;

	ss.str(entry.substr(idx));
	if (getline(ss, token, delim)) {
		size_t numberIdx;
		next_idx = token.size() + idx + 1;
		Trim(token);
		base = GetBaseAt(token, numberIdx);
		if (base != 2 && base != 16 && base != 10) return false;
		convstat = stringToDouble(token.c_str(), r, base);
		//std::cout << "anirb: Lexer::GetNumberAt -- r = " << *r << "\r\n";
		if (numberIdx != 0) token = token.substr(numberIdx);
		if (convstat) return true;
	}
	next_idx = token.size() + idx + 1;
	return false;
}

bool Lexer::ParseNumber(const string& entry, size_t idx, size_t& next_idx, vector<SynError>& errors,
						vector<SynElement>& elements) {
	double r;
	int base = 10;
	if (GetNumberAt(entry, idx, next_idx, base, &r)) {
		//std::cout << "anirb: Lexer::ParseNumber -- got double = " << r << "\r\n";
		elements.push_back({.type = kNumber, .re = r, .re_base = base});
		return true;
	} else {
		errors.push_back({entry.size(), "unterminated number"});
		return false;
	}
}

bool Lexer::ParseComplex(const string& entry, size_t idx, size_t& next_idx,
						 vector<SynError>& errors __attribute__((unused)), vector<SynElement>& elements) {
	double re;
	double im;
	int re_base, im_base = 10;
	if (idx + 1 == entry.size()) {
		elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
		next_idx = entry.size();
		return true;  // complex format error, return a symbol
	}
	if (!GetNumberAt(entry, idx + 1, next_idx, re_base, &re, ',')) {
		elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
		next_idx = entry.size();
		return true;  // complex format error, return a symbol
	}

	size_t i = next_idx;
	if (i >= entry.size()) {
		elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
		next_idx = entry.size();
		return true;  // complex format error, return a symbol
	}

	if (!GetNumberAt(entry, i, next_idx, im_base, &im, ')')) {
		elements.push_back({.type = kSymbol, .value = entry.substr(idx, entry.size() - idx)});
		next_idx = entry.size();
		return true;  // complex format error, return a symbol
	}
	elements.push_back({.type = kComplex, .re = re, .im = im, .re_base = re_base, .im_base = im_base});
	next_idx++;
	return true;
}

bool Lexer::ParseReserved(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements,
						  map<string, ReservedWord>& keywords) {
	stringstream ss(entry.substr(idx));
	string token;
	ss >> token;

	auto resa = keywords.find(token);
	if (resa != keywords.end()) {
		elements.push_back({.type = resa->second.type, .value = token, .fn = resa->second.fn});
		next_idx = token.size() + idx;
		return true;
	}
	return false;
}

bool Lexer::ParseUnknown(const string& entry, size_t idx, size_t& next_idx, vector<SynElement>& elements) {
	stringstream ss(entry.substr(idx));
	string token;
	ss >> token;
	elements.push_back({.type = kSymbol, .value = token, .auto_eval = true});
	next_idx = token.size() + idx;
	return true;
}

#pragma once
inline float float_mod(float value, int modulo) {
	int tmpVal = (int)value;

	int mod = tmpVal % modulo;
	float rest = value - (float)tmpVal;

	return tmpVal + rest;
}

#include <string>
#include <vector>
inline std::vector<std::string> splitString(std::string val, std::string delimiter) {
	std::vector<std::string> vals;
	size_t pos = 0;
	std::string token;
	while ((pos = val.find(delimiter)) != std::string::npos) {
		token = val.substr(0, pos);
		vals.push_back(token);
		val.erase(0, pos + delimiter.length());
	}
	vals.push_back(val);

	return vals;
}
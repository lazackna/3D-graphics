#pragma once
inline float float_mod(float value, int modulo) {
	int tmpVal = (int)value;

	int mod = tmpVal % modulo;
	float rest = value - (float)tmpVal;

	return mod + rest;
}

#include <string>
#include <vector>
inline std::vector<std::string> splitString(std::string val, std::string delimiter) {
	std::vector<std::string> vals;
	if (delimiter.empty()) {
		vals.push_back(val);
		return vals;
	}
	size_t pos = 0;
	std::string token;
	while ((pos = val.find(delimiter)) != std::string::npos) {
		token = val.substr(0, pos);
		vals.push_back(token);
		val.erase(0, pos + delimiter.length());
	}
	if(!val.empty())
	vals.push_back(val);

	return vals;
}
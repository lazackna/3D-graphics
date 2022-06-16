#pragma once

/// <summary>
/// Used to calculate modulo of a float with the remaining decimals returned as well.
/// </summary>
/// <param name="value"></param> Is the value that needs to be modulo'd
/// <param name="modulo"></param> Is the modulo factor.
/// <returns>A float modulo. This will have the remaining decimal numbers as well.</returns>
inline float float_mod(float value, int modulo) {
	int tmpVal = (int)value;

	int mod = tmpVal % modulo;
	float rest = value - (float)tmpVal;

	return mod + rest;
}

#include <string>
#include <vector>
/// <summary>
/// Used to split a string into a vector.
/// </summary>
/// <param name="val"></param> The value that needs to be split.
/// <param name="delimiter"></param> The value on which val must be split.
/// <returns>A vector of type string</returns>
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
#pragma once
inline float float_mod(float value, int modulo) {
	int tmpVal = (int)value;

	int mod = tmpVal % modulo;
	float rest = value - (float)tmpVal;

	return tmpVal + rest;
}
#pragma once
#include "UUIDGenerator.h"

class UUIDGeneratorSingleton {

public:
	UUIDGenerator* UUIDGenerator;

public:
	static UUIDGeneratorSingleton& getInstance() {
		static UUIDGeneratorSingleton instance;
		return instance;
	}
private:
	UUIDGeneratorSingleton() {}

	//WindowSingleton(WindowSingleton const&);
	//void operator=(WindowSingleton const&);

public:
	UUIDGeneratorSingleton(UUIDGeneratorSingleton const&) = delete;
	void operator=(UUIDGeneratorSingleton const&) = delete;
};
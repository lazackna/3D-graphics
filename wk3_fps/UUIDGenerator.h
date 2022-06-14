#pragma once
class UUIDGenerator {
private:
	

public:
	unsigned long createUUID() {
		static unsigned long UUIDCounter = 0;
		return UUIDCounter++;
	}
};
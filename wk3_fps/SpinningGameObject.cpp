#include "SpinningGameObject.h"

void SpinningGameObject::update(float deltatime) {
	rotation.x += spin.x * deltatime;
	rotation.y += spin.y * deltatime;
	rotation.z += spin.z * deltatime;
}



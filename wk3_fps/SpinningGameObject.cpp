#include "SpinningGameObject.h"

void SpinningGameObject::update(float deltatime) {
	rotation.x += spin.x * deltatime / 50;
	rotation.y += spin.y * deltatime / 50;
	rotation.z += spin.z * deltatime / 50;

	if (rotation.x > 360) rotation.x = 0;
	if (rotation.y > 360) rotation.y = 0;
	if (rotation.z > 360) rotation.z = 0;
}



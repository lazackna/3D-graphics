#include "SpinningGameObject.h"

void SpinningGameObject::update(float deltatime) {
	rotation.x += spin.x * deltatime / 10;
	rotation.y += spin.y * deltatime / 10;
	rotation.z += spin.z * deltatime / 10;

	GameObject::update(deltatime);
}



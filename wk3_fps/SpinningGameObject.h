#pragma once
#include "GameObject.h"
class SpinningGameObject : public GameObject
{
public:
	glm::vec3 spin;
public:
	SpinningGameObject(glm::vec3 _spin) : GameObject(), spin(_spin) {}

	void update(float deltatime);
};


#pragma once
#include "GameObject.h"
class MovableGameObject : public GameObject
{
public:
	MovableGameObject() : GameObject() {}
	
	void update(float deltatime);

};


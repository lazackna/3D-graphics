#pragma once

#include <string>

#include "GameObjects.h"

class GameObject;
class GameObjectFactory
{
public:
	static GameObject* createGameObject(std::string scenePath, std::string objectType, std::string UUID);
	static bool saveGameObject(std::string scenePath, GameObject* gameObject);
};


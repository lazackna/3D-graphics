#pragma once
//#include "GameObject.h"
#include "MovableGameObject.h"
#include "SpinningGameObject.h"

class GameObjectFactory
{
public:
	
	static GameObject * getGameObject(std::string type);
	static GameObject* getGameObject(std::string type, std::vector<std::string> extraData);
};


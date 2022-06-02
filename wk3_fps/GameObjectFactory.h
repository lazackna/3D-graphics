#pragma once
#include "json.hpp"
#include "MovableGameObject.h"
#include "SpinningGameObject.h"

class GameObjectFactory
{
public:
	
	static GameObject * getGameObject(std::string type);
	static GameObject* getGameObject(std::string type, nlohmann::json extradata);
};


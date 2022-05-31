#pragma once
#include "GameObjectFactory.h"
class SceneManager
{
	bool save(std::string sceneName, std::vector<GameObject*> gameobjects);
	bool load(std::string sceneName, std::vector<GameObject*>& gameobjects)
};


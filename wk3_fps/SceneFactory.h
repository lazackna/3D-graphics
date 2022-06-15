#pragma once

#include <vector>
#include <string>
class GameObject;
class SceneFactory
{
private:
	const int linesPerObject = 2;
public:
	static std::vector<GameObject*> createScene(std::string sceneName);
};


#include "GameObjectFactory.h"
#include "util.h"

GameObject * GameObjectFactory::getGameObject(std::string type) {
	if (type._Equal("GameObject")) { 
		return new GameObject(); 
	}
	else if (type._Equal("MovableGameObject")) { 
		return new MovableGameObject(); 
	}
	else if (type._Equal("SpinningGameObject")) {
		return new SpinningGameObject(glm::vec3(0, 1, 0));
	}
	
	return nullptr;
}

GameObject* GameObjectFactory::getGameObject(std::string type, std::vector<std::string> extraData) {
	if (type._Equal("GameObject")) {
		return new GameObject();
	}
	else if (type._Equal("MovableGameObject")) {
		return new MovableGameObject();
	}
	else if (type._Equal("SpinningGameObject")) {
		std::vector<std::string> values = split(extraData[0], ",");
		return new SpinningGameObject(glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2])));
	}

	return nullptr;
}

#include "GameObjectFactory.h"

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

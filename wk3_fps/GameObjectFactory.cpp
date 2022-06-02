#include "GameObjectFactory.h"
//#include "SerializationEnabler.h"
#include "json.hpp"
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

GameObject* GameObjectFactory::getGameObject(std::string type, nlohmann::json extradata) {
	nlohmann::json j = extradata;
	if (type._Equal("GameObject")) {
		return new GameObject();
	}
	else if (type._Equal("MovableGameObject")) {
		return new MovableGameObject();
	}
	else if (type._Equal("SpinningGameObject")) {
		std::vector<float> spin = j["spin"];
	
		return new SpinningGameObject(glm::vec3(spin[0], spin[1], spin[2]));
	}

	return nullptr;
}

#include "GameObjectFactory.h"

#include "GameObject.h"
#include <fstream>
#include <iostream>
#include "SerializationEnabler.h"
#include "json.hpp"
#include "UUIDGeneratorSingleton.h"

using json = nlohmann::json;

GameObject* GameObjectFactory::createGameObject(std::string scenePath, std::string objectType, std::string UUID) {
	GameObject* gameobject = nullptr;

	std::ifstream input = std::ifstream(scenePath + "/" + UUID + ".gmobj");
	if (!input.is_open()) {
		std::cout << "Could not find gameobject file!" << std::endl;
		return gameobject;
	}
	
	nlohmann::json data = nlohmann::json::parse(input);
	nlohmann::json extraData;
	if (data.contains("extradata")) extraData = data["extradata"];
	
	if (objectType._Equal("GameObject")) {
		gameobject = new GameObject();
	}
	else if (objectType._Equal("MovableGameObject")) {
		gameobject = new MovableGameObject();
	}
	else if (objectType._Equal("SpinningGameObject")) {
		glm::vec3 spin = extraData["spin"];
		gameobject = new SpinningGameObject(spin);
	}
	else {
		return nullptr;
	}
	json test;
	test["key"] = 1;
	//currently calling any [] on json here crashes the application
	int uuid = data["UUID"];
	gameobject->UUID = uuid;
	UUIDGeneratorSingleton::getInstance().UUIDGenerator->createUUID();
	if (data.contains("model")) {
		gameobject->model = new ObjModel(data["model"]);
	}
	else {
		gameobject->vertexes = data["vertexes"];
	}
	gameobject->transform = data["transform"];
	gameobject->rotation = data["rotation"];
	gameobject->scale = data["scale"];

	return gameobject;
}

bool GameObjectFactory::saveGameObject(std::string scenePath, GameObject* gameObject) {
	std::string garbageDump;
	std::string temp = "/" + std::to_string(gameObject->UUID);
	std::string objectFileName = scenePath + temp;		
	std::ofstream output = std::ofstream(objectFileName + ".gmobj");
	if (!output.is_open()) {
		std::cout << "Failed to create a save file!" << std::endl;
		return false;
	}
	json object;
	json extraData;
	
	std::string typeName = typeid(*gameObject).name();
	typeName.replace(0, 6, garbageDump);
	object["type"] = typeName;
	object["UUID"] = gameObject->UUID;
	if (gameObject->model) {
		object["model"] = gameObject->model->modelName;
	}
	else {
		object["vertexes"] = gameObject->vertexes;
	}

	//add transform, rotation, scale
	object["transform"] = gameObject->transform;
	object["rotation"] = gameObject->rotation;
	object["scale"] = gameObject->scale;

	if (SpinningGameObject* so = dynamic_cast<SpinningGameObject*>(gameObject)) {

		extraData["spin"] = so->spin;

	}
	if(!extraData.empty())
	object["extradata"] = extraData;
	output << object.dump();
	output.close();
	return true;
}
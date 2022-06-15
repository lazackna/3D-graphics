#include "SceneManager.h"
#include <fstream>
#include <iostream>
//#include "util.h"
#include "UUIDGeneratorSingleton.h"
#include "SceneFactory.h"
//#include "SerializationEnabler.h"
#include <filesystem>

namespace fs = std::filesystem;

void createDirectory(std::string path) {
	if (!fs::is_directory(path) || !fs::exists(path)) {
		fs::create_directory(path);
	}
}

bool SceneManager::save(std::string sceneName, std::vector<GameObject*> gameobjects) {
	std::cout << "Loading " << sceneName << std::endl;
	std::string dirName = "scenes/" + sceneName;
	if (dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if (dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if (sceneName == dirName)
		dirName = "";
	
	std::ofstream output(dirName + "/" + sceneName + ".scene");

	if (!output.is_open()) {
		std::cout << "Failed to create a save file!" << std::endl;
		return false;
	}
	std::string garbageDump;
	// Save the scene
	for (int i = 0; i < gameobjects.size(); i++) {
		GameObject* gameobject = gameobjects[i];
		if (gameobject->UUID == ULONG_MAX) gameobject->UUID = UUIDGeneratorSingleton::getInstance().UUIDGenerator->createUUID();
		output << "UUID:" + std::to_string(gameobject->UUID) << std::endl;
		std::string typeName = typeid(*gameobject).name();
		typeName.replace(0, 6, garbageDump);
		if (i >= gameobjects.size() - 1) {
			output << "Type:" + typeName;
		}
		else {
			output << "Type:" + typeName << std::endl;
		}
	}
	output.close();

	std::string objectsRoot = "scenes/" + sceneName;
	createDirectory(objectsRoot);
	// Save all objects.
	for (int i = 0; i < gameobjects.size(); i++) {
		GameObjectFactory::saveGameObject(objectsRoot, gameobjects[i]);
	//	GameObject* o = gameobjects[i];
	//	
	//	std::string temp = "/" + std::to_string(o->UUID);
	//	std::string objectFileName = objectsRoot + temp;
	//	
	//	output = std::ofstream(objectFileName + ".gmobj");
	//	if (!output.is_open()) {
	//		std::cout << "Failed to create a save file!" << std::endl;
	//		return false;
	//	}
	//	json object;
	//	json extraData;
	//
	//	std::string typeName = typeid(*o).name();
	//	typeName.replace(0, 6, garbageDump);
	//	object["type"] = typeName;
	//	object["UUID"] = o->UUID;
	//	if (o->model) {
	//		object["model"] = o->model->modelName;
	//	}
	//	else {

	//		object["vertexes"] = o->vertexes;
	//	}

	//	//add transform, rotation, scale
	//	object["transform"] = o->transform;
	//	object["rotation"] = o->rotation;
	//	object["scale"] = o->scale;

	//	if (SpinningGameObject* so = dynamic_cast<SpinningGameObject*>(o)) {

	//		extraData["spin"] = so->spin;

	//	}
	//	if(!extraData.empty())
	//	object["extradata"] = extraData;
	//	output << object.dump();
	//	output.close();
	}

	return true;
}
bool SceneManager::load(std::string sceneName, std::vector<GameObject*>& gameobjects) {
	// load object type -> transform -> rotation -> scale -> extra variables

	std::cout << "Loading " << sceneName << std::endl;
	std::string dirName = "scenes/" + sceneName;
	if (dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if (dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if (sceneName == dirName)
		dirName = "";

	std::ifstream input("scenes/" + sceneName + ".scene");
	if (!input.is_open()) {
		std::cout << "Could not find save file!" << std::endl;
		return false;
	}
	
	gameobjects = SceneFactory::createScene("scenes/" + sceneName);
	/*json extradata;

	json j = json::parse(input);
	
	json objects = j["gameobjects"];

	for (int i = 0; i < objects.size(); i++) {
		json object = objects[i];

		std::string type = object["type"];

		GameObject* gameobject = GameObjectFactory::createGameObject(object["type"], object["extradata"]);
		if (object.contains("model")) {
			gameobject->model = new ObjModel(object["model"]);
		}
		else {
			gameobject->vertexes = object["vertexes"];
		}
		gameobject->transform = object["transform"];
		gameobject->rotation = object["rotation"];
		gameobject->scale = object["scale"];

		gameobjects.push_back(gameobject);
	}*/
	return true;
}
#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include "util.h"

static inline std::string createVertexString(GameObject* o) {
	std::string output = "[";
	for (int i = 0; i < o->vertexes.size(); i++) {
		tigl::Vertex v = o->vertexes[i];
		// Need to add tigl::vertex to string
		std::string data = reinterpret_cast<char*>(&v);
		output.append(data);
		if (i != o->vertexes.size() - 1) {
			output.append(",");
		}
	}
	output.append("]");
	return output;
}

static inline std::vector<tigl::Vertex> vertexStringToVector(std::string vertexData) {
	//return reinterpret_cast<std::vector<tigl::Vertex>>(vertexData);
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

	std::ofstream output(dirName + "/" + sceneName);

	if (!output.is_open()) {
		std::cout << "Failed to create a save file!" << std::endl;
		return false;
	}

	std::string s;
	for (int i = 0; i < gameobjects.size(); i++) {
		GameObject* o = gameobjects[i];
		std::string typeName = typeid(*o).name();
		
		typeName.replace(0, 6, s);
		if (i == 0)
			output << "type:" << typeName;
		else
			output << std::endl << "type:" << typeName;
		if (o->model) {
			output << std::endl << "model:" << o->model->modelName;
		}
		else {
			output << std::endl << "vertexes:" << createVertexString(o);
		}
		
		//add transform, rotation, scale
		output << std::endl << "transform:" << o->transform[0] << ',' << o->transform[1] << ',' << o->transform[2];
		output << std::endl << "rotation:" << o->rotation[0] << ',' << o->rotation[1] << ',' << o->rotation[2];
		output << std::endl << "scale:" << o->scale[0] << ',' << o->scale[1] << ',' << o->scale[2];
		if (SpinningGameObject* so = dynamic_cast<SpinningGameObject*>(o)) {
			output << std::endl << "spin:" << so->spin[0] << ',' << so->spin[1] << ',' << so->spin[2];
		}
	}
	
	output.close();
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

	std::ifstream input("scenes/" + sceneName);
	if (!input.is_open()) {
		std::cout << "Failed to create a save file!" << std::endl;
		return false;
	}
	bool firstObject = false;
	bool newObject = false;
	/*if (!input.eof()) {
		std::string firstType;
		std::getline(input, firstType);
		if (split(firstType, ":")[0]._Equal("type")) {
			
		}
	}*/
	std::string currentType, model, transform, rotation, scale;
	std::vector<std::string> extraData;
	
	while (!input.eof()) {
		
		std::string line;
		std::getline(input,line);
		if (line.empty()) return false;
		std::vector<std::string> values = split(line, ":");

		if (values[0]._Equal("type")) {
			if (!firstObject) { 
				firstObject = true; 
				currentType = values[1];
			}
			else newObject = true;
			
		} else if (values[0]._Equal("model")) {
			model = values[1];
		} else if (values[0]._Equal("transform")) {
			transform = values[1];
		} else if (values[0]._Equal("rotation")) {
			rotation = values[1];
		} else if (values[0]._Equal("scale")) {
			scale = values[1];
		} else {
			extraData.push_back(values[1]);
		}

		if (newObject) {
			GameObject* object = GameObjectFactory::getGameObject(currentType, extraData);
			if (!model.empty()) object->model = new ObjModel(model);
			object->transform = stringToVec3(transform);
			object->rotation = stringToVec3(rotation);
			object->scale = stringToVec3(scale);
			
			gameobjects.push_back(object);
			
			currentType = values[1];
			model = "";
			transform = "";
			rotation = "";
			scale = "";
			newObject = false;
		}
		
	}
	GameObject* object = GameObjectFactory::getGameObject(currentType, extraData);
	if (!model.empty()) object->model = new ObjModel(model);
	object->transform = stringToVec3(transform);
	object->rotation = stringToVec3(rotation);
	object->scale = stringToVec3(scale);

	gameobjects.push_back(object);


	return false;
}
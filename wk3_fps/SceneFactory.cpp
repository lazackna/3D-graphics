#include "SceneFactory.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include <fstream>
#include "util.h"
#include <iostream>

std::vector<GameObject*> SceneFactory::createScene(std::string sceneName) {
	std::vector<GameObject*> objects;

	std::ifstream input = std::ifstream(sceneName + ".scene");
	if (!input.is_open()) {
		std::cout << "Could not find scene file!" << std::endl;
		return objects;
	}
	int currentObjectLine = -1;
	std::string currenttype;
	std::string currentUUID;
	while (!input.eof()) {
		std::string line;
		std::getline(input, line);

		std::vector<std::string> values = splitString(line, ":");
		currentObjectLine++;
		switch (currentObjectLine) {
		case 2:
			//new object so automatically flow over to case 0.
			currentObjectLine = 0;
			GameObjectFactory::createGameObject(sceneName, currenttype, currentUUID);
		case 0:
			// new uuid
			currentUUID = values[1];
			break;

		case 1:
			// new type
			currenttype = values[1];
			break;
		}
	}

	input.close();

	return objects;
}
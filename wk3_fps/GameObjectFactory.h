#pragma once

#include <string>

#include "GameObjects.h"

class GameObject;
class GameObjectFactory
{
public:
	/// <summary>
	/// Used to create a gameobject through a file.
	/// </summary>
	/// <param name="scenePath"></param> The path of the scene the object resides in.
	/// <param name="objectType"></param> The object type the object is.
	/// <param name="UUID"></param> The UUID of the gameobject. This is used to look up which file to use.
	/// <returns>A constructed Gameobject*. If a file was not found a nullptr is returned. If the objectType is not known in the
	/// system a nullptr will be returned.</returns>
	static GameObject* createGameObject(std::string scenePath, std::string objectType, std::string UUID);
	
	/// <summary>
	/// Used to save a gameobject. Method is placed in this class to prevent multiple definition with nlohmann.
	/// </summary>
	/// <param name="scenePath"></param> The scene the object needs to be saved in.
	/// <param name="gameObject"></param> The gameobject that needs to be saved.
	/// <returns>A boolean that indicates if the save was succesfull.</returns>
	static bool saveGameObject(std::string scenePath, GameObject* gameObject);
};


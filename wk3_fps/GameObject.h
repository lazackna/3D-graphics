#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "tigl.h"
#include "ObjModel.h"
#include "GLFW/glfw3.h"
#include "WindowSingleton.h"
class GameObject
{
public:
	glm::vec3 transform = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	std::vector<tigl::Vertex> vertexes;
	ObjModel * model;
protected:
	GLFWwindow* window;

public:
	GameObject() {window = WindowSingleton::getInstance().window;}
	/*GameObject(std::vector<tigl::Vertex> _vertexes) : vertexes(_vertexes) {}
	GameObject(std::string& fileName) : model(new ObjModel(fileName)) { }*/
	void draw();
	virtual void update(float deltaTime);
};


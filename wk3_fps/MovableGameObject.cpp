#include "MovableGameObject.h"

void MovableGameObject::update(float deltatime) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		transform += glm::vec3(0, 0, -((1 * deltatime) / 100));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		transform += glm::vec3(0, 0, ((1 * deltatime) / 100));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		transform += glm::vec3((1 * deltatime) / 100, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		transform += glm::vec3(-((1 * deltatime) / 100), 0, 0);
	}

}

#include "GameObject.h"

void GameObject::update(float deltatime) {

}

void GameObject::draw() {
	glm::mat4 mat(1.0f);
	mat = glm::scale(mat, scale);
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	mat = glm::translate(mat, transform);
	tigl::shader->setModelMatrix(mat);
	if (model) model->draw();
	else tigl::drawVertices(GL_TRIANGLES, vertexes);
}
/////////
//EISEN
// De code gebruikt geen overbodige #defines en bevat geen magische constanten
// 
// De code bevat geen leaks van memory of andere resources
// 
// De code gebruikt een zinne wijze overerving en virtuele functies.
// Zie GameObject.h en GameObject.cpp. Dit is de basis klasse.
// MovableGameObject en SpinningGameObject erven hiervan over.
// UML diagram staat in documentation/GameObject.png.
// 
// De code bevat een herbruikbaar onderdeel dat is gedocumenteerd is met Doxgyen
// en getest is met een geautomatiseerde unit-test.
// Er wordt gebruik gemaakt van de native unit test van visual studio.
// Kijk naar util.h. Deze is compleet gedocumenteerd. De split_string methode
// wordt constant gebruikt bij het inladen van een scene.
// 
// De code gebruikt waar mogelijk en ziggen STL algoritmen en containers.
// Overal wordt er gebruik gemaakt van de vector container.
// In ObjModel.cpp in de methode draw op regel 236 wordt er gebruik gemaakt
// van het for each algoritme om door een map heen te gaan.
// 
// De applicatie leest gegevens in vanuit een tekst file naar in-memory data, 
// volgens het factory pattern.
// Dit wordt gedaan in SceneFactory. h/cpp en in GameObjectFactory. h/cpp.
// Bij de scene factory wordt de naam van de scene meegegeven die vervolgens
// wordt gebruikt het bestand van de scene in te lezen.
// Deze data wordt vervolgens gebruikt om de GameObjects in te lezen die
// allemaal in een los bestand staan in de folder met de scene naam.
// 
////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "ObjModel.h"
#include "WindowSingleton.h"
#include "UUIDGeneratorSingleton.h"
#include "SceneManager.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

const std::string selectedScene = "test2";

GLFWwindow* window;
ObjModel* model;
int width, height, bpp;
int mapWidth, mapHeight, mapbpp;
GLuint textureId;

std::vector<GameObject*> gameobjects;
bool enableFog = false;

double lastTime, deltaTime;

int map[20][10][20];

void init();
//void initMap();
void update(double deltatime);
void draw();
//void drawMap();
void loadAtlas();
void drawCube(int index);
GameObject* createCube();
void createGround();
void createCubeWithTexture(int xb, int yb, int sizeX, int sizeY);
void rotateDegrees(glm::mat4& model, const glm::vec3 rotation);

int main(void)
{   
    if (!glfwInit())
        throw "Could not initialize glwf";
    int count;
    SceneManager s = SceneManager();
    
    WindowSingleton::getInstance().window = glfwCreateWindow(1000, 800, "Hello World", NULL, NULL);
    window = WindowSingleton::getInstance().window;
    if (!window)
    {
        
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    
    init();
    s.load(selectedScene, gameobjects);

    while (!glfwWindowShouldClose(window))
    {
        double nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime);
        lastTime = nowTime;
        update(deltaTime);
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    s.save(selectedScene, gameobjects);

    for (GameObject* g : gameobjects) {
        delete g;
    }
	glfwTerminate();

    return 0;
}

FpsCam* camera;

void init()
{

    int value[10];
    glEnable(GL_DEPTH_TEST);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    
    camera = new FpsCam(window);

    tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);
    tigl::shader->setLightDirectional(0, false);
    tigl::shader->enableFog(enableFog);
    tigl::shader->setLightPosition(0, {0,0,0});
    tigl::shader->setLightDiffuse(0, { 0.5f,0.5f,0.5f });
    tigl::shader->setLightAmbient(0, { 0.5f, 0.5f, 0.5f });
 
}

bool fogPressed = false;
void update(double deltatime)
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fogPressed) {
        enableFog = !enableFog;
        fogPressed = true;
        tigl::shader->enableFog(enableFog);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && fogPressed) {
        fogPressed = false;
    }

    camera->update(deltatime);   

    for (auto& object : gameobjects) object->update(deltaTime);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    tigl::shader->setFogColor(glm::vec3(0.3f, 0.4f, 0.6f));
    tigl::shader->setFogLinear(0, 4);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glEnable(GL_DEPTH_TEST);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));
    
    tigl::shader->enableTexture(false);
    tigl::shader->enableLighting(true);

    for (auto& object : gameobjects) 
    { 
        object->draw(); 
    }
   
}

void createGround() {
    GameObject* groundObject = GameObjectFactory::createGameObject("scenes/" + selectedScene, "GameObject", 
        std::to_string(UUIDGeneratorSingleton::getInstance().UUIDGenerator->createUUID()));
    groundObject->transform = glm::vec3(0, -1, 0);
    groundObject->scale = glm::vec3(20, 1, 20);
    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 1, 0)));//
    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0, 0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 1, 0)));//

    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0, 0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
    groundObject->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0, 0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
    gameobjects.push_back(groundObject);

}

GameObject* createCube() {
    
    GameObject* cube = new GameObject();
    cube->UUID = UUIDGeneratorSingleton::getInstance().UUIDGenerator->createUUID();
   
    //top
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 1, 0)));

    //bottom
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(0, -1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, -1, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, -1, 0)));

    //back
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 0, -1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 0, -1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 0, -1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 0, -1)));

    //front
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));

    //left
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(-1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec2(1, 1), glm::vec3(-1, 0, 0)));

    //right
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 1.0, 1.0), glm::vec2(0, 1), glm::vec3(1, 0, 0)));
    cube->vertexes.push_back(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(1, 0, 0)));

    return cube;
}

void rotateDegrees(glm::mat4& model, const glm::vec3 rotation) {
    glm::vec4 pos = model[3];
    model = glm::mat4(1.0f);
    model[3] = pos;
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
}
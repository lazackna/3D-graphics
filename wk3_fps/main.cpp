
//#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "ObjModel.h"
#include "WindowSingleton.h"
#include "GameObjectFactory.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")


GLFWwindow* window;
ObjModel* model;
int width, height, bpp;
int mapWidth, mapHeight, mapbpp;
GLuint textureId;

std::vector<GameObject*> gameobjects;

double lastTime, deltaTime;

int map[20][10][20];

void init();
//void initMap();
void update(double deltatime);
void draw();
//void drawMap();
void loadAtlas();
void drawCube(int index);
void createCube();
void createGround();
void createCubeWithTexture(int xb, int yb, int sizeX, int sizeY);
void rotateDegrees(glm::mat4& model, const glm::vec3 rotation);

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    int count;

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

    createGround();

    GameObject* testObject = GameObjectFactory::getGameObject("MovableGameObject");
    GameObject* spinningObject = GameObjectFactory::getGameObject("SpinningGameObject");
    gameobjects.push_back( testObject);
    gameobjects.push_back( spinningObject);
    
    
    testObject->model = new ObjModel("models/steve/Steve.obj");
    spinningObject->model = testObject->model;
    //loadAtlas();
    tigl::shader->enableLighting(true);
    //tigl::shader->enableColorMult(false);
    tigl::shader->setLightCount(1);
    tigl::shader->setLightDirectional(0, true);
    tigl::shader->setLightPosition(0, {0,20,0});
    tigl::shader->setLightDiffuse(0, { 0.5f,0.5f,0.5f });
    /*tigl::shader->setLightAmbient(0, { 100,146,0 });
    tigl::shader->setLightSpecular(0, { 0,100,20 });*/

    
}

void update(double deltatime)
{
    camera->update(deltatime);   

    for (auto& object : gameobjects) object->update(deltaTime);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
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
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, { 1,0,0 });
    tigl::shader->setModelMatrix(mat);
    tigl::shader->setLightDirectional(0, true);
    tigl::shader->setLightPosition(0, { 1,20,0 });
    tigl::shader->setLightDiffuse(0, { 0.5f,0.5f,0.5f });
    createCube();

    for (auto& object : gameobjects) object->draw();
   
}

void createGround() {
    GameObject* groundObject = GameObjectFactory::getGameObject("GameObject");
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

//void drawMap() {
//    glBindTexture(GL_TEXTURE_2D, textureId);
//    for (int x = 0; x < 20; x++)
//    {
//        for (int y = 0; y < 10; y++)
//        {
//            for (int z = 0; z < 20; z++)
//            {
//                glm::vec3 position(x,y -7.4,z);
//                glm::mat4 mat(1.0f);
//                mat = glm::scale(mat, glm::vec3(0.2f, 0.2f, 0.2f));
//                mat = glm::translate(mat, position);
//                
//                tigl::shader->setModelMatrix(mat);
//                drawCube(map[x][y][z]);
//            }
//        }
//    }
//}

//void loadAtlas() {
//    glGenTextures(1, &textureId);
//    glBindTexture(GL_TEXTURE_2D, textureId);
//    imgData = stbi_load("Atlas.png", &width, &height, &bpp, 4);
//
//    glTexImage2D(GL_TEXTURE_2D,
//        0,
//        GL_RGBA,
//        width,
//        height,
//        0,
//        GL_RGBA,
//        GL_UNSIGNED_BYTE,
//        imgData);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    stbi_image_free(imgData);
//}

//void drawCube(int index) {
//    if (index < 0) return;
//    int x = index % horizontalItems;
//    int y = index / horizontalItems;
//
//    int atlasX = x * horizontalItems;
//    int atlasY = y * verticalItems;
//    int sizeX = width / horizontalItems;
//    int sizeY = height / verticalItems;
//    createCubeWithTexture(atlasX, atlasY, horizontalItems, verticalItems);
//}

//void createCubeWithTexture(int xb, int yb, int sizeX, int sizeY) {
//
//    // xb and yb = (0,0)
//    glm::vec2 botL(xb / (double)width, yb / (double)height);
//    glm::vec2 botR((xb + sizeX) / (double)width, yb / (double)height);
//    glm::vec2 topL(xb / (double)width, (yb + sizeY) / (double)height);
//    glm::vec2 topR((xb + sizeX) / (double)width, (yb + sizeY) / (double)height);
//
//    tigl::begin(GL_QUADS);
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), botR, glm::vec4(1.0, 1.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), botL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), topL, glm::vec4(0.0, 1.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), topR, glm::vec4(1.0, 1.0, 1.0, 1.0)));
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), botR, glm::vec4(1.0, 0.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), botL, glm::vec4(0.0, 0.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), botR, glm::vec4(1.0, 1.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5),botL, glm::vec4(0.0, 1.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), botR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), botL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), topL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), topR, glm::vec4(1.0, 1.0, 0.0, 1.0)));
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), botR, glm::vec4(0.0, 1.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), botL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), topR, glm::vec4(0.0, 0.0, 1.0, 1.0)));
//
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), botR, glm::vec4(1.0, 1.0, 0.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), botL, glm::vec4(1.0, 1.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), topL, glm::vec4(1.0, 0.0, 1.0, 1.0)));
//    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
//    tigl::end();
//}

void createCube() {
    
    tigl::begin(GL_QUADS);
    
    //top
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 1, 0)));

    //bottom
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(0, -1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, -1, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, -1, 0)));

    //back
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(0 ,0 ,-1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 0, -1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 0, -1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 0, -1)));

    //front
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(0,0,1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));

    //left
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, 0.5), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(1, 0), glm::vec3(-1,0,0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, 0.5, -0.5), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, -0.5), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(0, 1), glm::vec3(-1, 0, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(-0.5, -0.5, 0.5), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec2(1, 1), glm::vec3(-1, 0, 0)));

    //right
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, -0.5), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1, 0), glm::vec3(1,0,0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(0, 0), glm::vec3(1, 0, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, 0.5), glm::vec4(1.0, 0.0, 1.0, 1.0), glm::vec2(0, 1), glm::vec3(1, 0, 0)));
    tigl::addVertex(Vertex::PCTN(glm::vec3(0.5, -0.5, -0.5), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(1, 1), glm::vec3(1, 0, 0)));
    tigl::end();
}

void rotateDegrees(glm::mat4& model, const glm::vec3 rotation) {
    glm::vec4 pos = model[3];
    model = glm::mat4(1.0f);
    model[3] = pos;
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
}
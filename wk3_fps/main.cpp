
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
stbi_uc * imgData, * mapImgData;

ObjModel* steve;
glm::mat4 steve1Mat(1.0f);
glm::mat4 steve2Mat(1.0f);
double steve2Y = 0;
double steve2YMove = 0.5;
glm::mat4 steve3Mat(1.0f);
int rotation = 0;
int horizontalItems = 16;
int verticalItems = 16;

MovableGameObject* testObject;
SpinningGameObject* spinningObject;

ObjModel* ground;

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
    
    steve = new ObjModel("models/steve/Steve.obj");
    steve1Mat = glm::scale(steve1Mat, glm::vec3(0.05f, 0.05f, 0.05f));
    steve1Mat = glm::translate(steve1Mat, glm::vec3(0, 0, 0));

    steve2Mat = glm::scale(steve1Mat, glm::vec3(0.05f, 0.05f, 0.05f));
    steve2Mat = glm::translate(steve1Mat, glm::vec3(2, 0, 2));

    steve3Mat = glm::scale(steve1Mat, glm::vec3(0.05f, 0.05f, 0.05f));
    steve3Mat = glm::translate(steve1Mat, glm::vec3(10, 2, 10));
    ground = new ObjModel("models/world/ground/ground.obj");
    testObject = (MovableGameObject*)GameObjectFactory::getGameObject("MovableGameObject");
    spinningObject = (SpinningGameObject*)GameObjectFactory::getGameObject("SpinningGameObject");

    
    testObject->model = new ObjModel("models/steve/Steve.obj");
    spinningObject->model = testObject->model;
    //loadAtlas();
    tigl::shader->enableLighting(true);
    
    tigl::shader->setLightCount(1);
    tigl::shader->setLightDirectional(0, false);
    tigl::shader->setLightPosition(0, {0,5,0});
    tigl::shader->setLightDiffuse(0, { 255,140,130 });
    //initMap();
}

//void initMap() {
//    for (int x = 0; x < 20; x++)
//    {
//        for (int y = 0; y < 10; y++)
//        {
//            for (int z = 0; z < 20; z++)
//            {
//                if (y < 5) {
//                    map[x][y][z] = 1;
//                }
//                else if (y == 5) {
//                    map[x][y][z] = 2;
//                }
//                else {
//                    map[x][y][z] = -1;
//                }
//            }
//        }
//    }
//}

void update(double deltatime)
{
    camera->update(window);
    rotation += 0.5 * deltaTime;
    if (rotation > 360) rotation = 0;
    steve2Y += steve2YMove * deltatime;
    if (steve2Y > 10) steve2YMove = -steve2YMove;
    if (steve2Y < 6) steve2YMove = -steve2YMove;
    testObject->update(deltaTime);
    spinningObject->update(deltatime);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));
    

   // tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);
   // tigl::shader->enableColor(true);
    
    //tigl::shader->enableLighting(true);
    


    glEnable(GL_DEPTH_TEST);
    //drawMap();
    //steve1Mat = glm::scale(steve1Mat, glm::vec3(0.1f, 0.1f, 0.1f));
   // rotateDegrees(steve1Mat, glm::vec3(0,rotation,0));
   // steve1Mat = glm::scale(steve1Mat, glm::vec3(0.1f, 0.1f, 0.1f));
   // tigl::shader->setModelMatrix(steve1Mat);
   // //steve->draw();

    glm::mat4 mat(1.0f);
   //// mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
   // steve2Mat += glm::translate(mat, glm::vec3(0, steve2Y, 0));
   // steve2Mat = glm::scale(steve2Mat, glm::vec3(0.1f, 0.1f, 0.1f));
   // tigl::shader->setModelMatrix(steve2Mat);
   // //steve->draw();

   // rotateDegrees(steve3Mat, glm::vec3(rotation, rotation, rotation));
   // steve3Mat = glm::scale(steve3Mat, glm::vec3(0.1f, 0.1f, 0.1f));
   // tigl::shader->setModelMatrix(steve3Mat);
   // //steve->draw();

   // mat = glm::mat4(1.0f);
   // mat = glm::scale(mat, { 0.1, 0.1, 0.1 });
   // mat = glm::translate(mat, { 0, -10, 0 });
   // tigl::shader->setModelMatrix(mat);
   // ground->draw();
    
    tigl::shader->enableTexture(true);
    tigl::shader->enableLighting(true);
    mat = glm::mat4(1.0f);
    mat = glm::translate(mat, { 1,0,0 });
    tigl::shader->setModelMatrix(mat);
    
    createCube();
    /*tigl::shader->enableLighting(false);
    tigl::shader->setModelMatrix(glm::mat4(1.0f));
    testObject->draw();
    spinningObject->draw();*/
    //createCube();
    //for (int i = 0; i < horizontalItems * verticalItems; i++) {
    //    glm::mat4 mat(1.0f);
    //    
    //    mat = glm::translate(mat , glm::vec3(i, 0, 0));
    //   // mat = glm::scale(mat, glm::vec3(0.5, 0.5, 0.5));
    //    tigl::shader->setModelMatrix(mat);

    //    drawCube(i);
    //}
   
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

void loadAtlas() {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    imgData = stbi_load("Atlas.png", &width, &height, &bpp, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        imgData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(imgData);
}

void drawCube(int index) {
    if (index < 0) return;
    int x = index % horizontalItems;
    int y = index / horizontalItems;

    int atlasX = x * horizontalItems;
    int atlasY = y * verticalItems;
    int sizeX = width / horizontalItems;
    int sizeY = height / verticalItems;
    createCubeWithTexture(atlasX, atlasY, horizontalItems, verticalItems);
}

void createCubeWithTexture(int xb, int yb, int sizeX, int sizeY) {

    // xb and yb = (0,0)
    glm::vec2 botL(xb / (double)width, yb / (double)height);
    glm::vec2 botR((xb + sizeX) / (double)width, yb / (double)height);
    glm::vec2 topL(xb / (double)width, (yb + sizeY) / (double)height);
    glm::vec2 topR((xb + sizeX) / (double)width, (yb + sizeY) / (double)height);

    tigl::begin(GL_QUADS);

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), botR, glm::vec4(1.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), botL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), topL, glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), topR, glm::vec4(1.0, 1.0, 1.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), botR, glm::vec4(1.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), botL, glm::vec4(0.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), botR, glm::vec4(1.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5),botL, glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), botR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), botL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), topL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), topR, glm::vec4(1.0, 1.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), botR, glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), botL, glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), topL, glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), topR, glm::vec4(0.0, 0.0, 1.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), botR, glm::vec4(1.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), botL, glm::vec4(1.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), topL, glm::vec4(1.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), topR, glm::vec4(1.0, 0.0, 0.0, 1.0)));
    tigl::end();
}

void createCube() {
    
    tigl::begin(GL_QUADS);
    
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1,0), glm::vec4(1.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0,0), glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0, 1), glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1, 1), glm::vec4(1.0, 1.0, 1.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1, 0), glm::vec4(1.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0, 0), glm::vec4(0.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 1), glm::vec4(1.0, 0.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1, 0), glm::vec4(1.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0, 0), glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0, 1), glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1, 1), glm::vec4(1.0, 0.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec4(1.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0, 1), glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1, 1), glm::vec4(1.0, 1.0, 0.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(1, 0), glm::vec4(0.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0, 0), glm::vec4(0.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec4(0.0, 0.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(1, 1), glm::vec4(0.0, 0.0, 1.0, 1.0)));

    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1, 0), glm::vec4(1.0, 1.0, 0.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, 0.5), glm::vec2(0, 0), glm::vec4(1.0, 1.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, 0.5), glm::vec2(0, 1), glm::vec4(1.0, 0.0, 1.0, 1.0)));
    tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 1), glm::vec4(1.0, 0.0, 0.0, 1.0)));
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
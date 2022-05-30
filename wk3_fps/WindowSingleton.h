#pragma once
#include <GLFW/glfw3.h>
class WindowSingleton {

public:
	GLFWwindow* window;

public:
	static WindowSingleton& getInstance() {
		static WindowSingleton instance;
		return instance;
	}
private :
	WindowSingleton() {}
	
	//WindowSingleton(WindowSingleton const&);
	//void operator=(WindowSingleton const&);

public:
	WindowSingleton(WindowSingleton const&) = delete;
	void operator=(WindowSingleton const&) = delete;
};
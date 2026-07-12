#pragma once

#include <GLFW/glfw3.h>
#include "model/Object3d.hpp"
#include "graphics/engine/Engine.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Window {
public:
	Window(Object3d& renderable);
	~Window();

	void initEngine(Object3d& renderable);
	void run();

	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLFWwindow* getWindow() { return this->m_window; }

private:
	GLFWwindow* m_window;
	Object3d& 	m_renderable;
	Engine* 	m_engine;
	float       m_deltaTime;

	void cleanGlfw();
	void processMovementInput();
};

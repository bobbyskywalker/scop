#pragma once

#include <GLFW/glfw3.h>
#include "../model/Object3d.hpp"
#include "../../inc/graphics/engine/Engine.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Window {
public:
	Window(Object3d& renderable);
	~Window();

	void initEngine(Object3d& renderable);
	void run(Object3d& renderable);

	static void error_callback(int error, const char* description);

	GLFWwindow* getWindow() { return this->m_window; }

private:
	GLFWwindow* m_window;
	Object3d& 	m_renderable;
	Engine* 	m_engine;

	void cleanGlfw();
	void processInput();
};

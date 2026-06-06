#pragma once

#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Window {
public:
	Window();
	~Window();

	void run();
	void processInput();

	static void error_callback(int error, const char* description);

	GLFWwindow* getWindow() { return this->m_window; }

private:
	GLFWwindow* m_window;

	void cleanGlfw();

};

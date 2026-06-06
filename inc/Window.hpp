#pragma once

#include <GLFW/glfw3.h>

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

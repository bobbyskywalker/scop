#include "../inc/glad/glad.h"
#include "GLFW/glfw3.h"
#include "../inc/Window.hpp"
#include <iostream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

Window::Window() {
	if (!glfwInit()) {
		std::cerr << "Error: GLFW initialization failure" << std::endl;
		std::exit(1);
	}
	glfwSetErrorCallback(Window::error_callback);
	this->m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "scop", NULL, NULL);
	if (!this->m_window) {
		std::cerr << "Error: GLFW window creation failure" << std::endl;
		std::exit(1);
	}
	glfwMakeContextCurrent(this->m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: GLAD initialization failure" << std::endl;
        std::exit(1);
    }

}

Window::~Window() {}

void Window::run() {
	while (!glfwWindowShouldClose(m_window)) {
		/* todo:
		* Process events
		* Clear screen - wipe previous frame
		* Update transformations - move/rotate object based on input
		* Draw
		* Swap buffers
		*/
		glfwPollEvents();
	}
	cleanGlfw();
}

void Window::error_callback(int error, const char* description) {
	std::cerr << "GLFW error: " << "[" << error << "] " << description << std::endl;
	std::exit(1);
}

void Window::cleanGlfw() {
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
}

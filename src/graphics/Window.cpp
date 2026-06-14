#include "../../inc/glad/glad.h"
#include "../../inc/graphics/Window.hpp"
#include "../../inc/graphics/engine/OpenGLEngine.hpp"
#include "GLFW/glfw3.h"
#include <cmath>
#include <cstddef>
#include <iostream>

// todo: window resizing
Window::Window(Object3d& renderable) : m_renderable(renderable), m_engine(nullptr) {
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
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->m_engine = new OpenGlEngine(renderable);
}

Window::~Window() {}

void Window::run() {
    while (!glfwWindowShouldClose(m_window)) {
   		this->m_engine->render(this->m_renderable);
        processInput();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    cleanGlfw();
}

void Window::processInput() {
	if(glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->m_window, true);
}

void Window::error_callback(int error, const char* description) {
	std::cerr << "GLFW error: " << "[" << error << "] " << description << std::endl;
	std::exit(1);
}

void Window::cleanGlfw() {
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
	delete this->m_engine;
}

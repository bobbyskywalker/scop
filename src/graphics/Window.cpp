#include "glad/glad.h"
#include "graphics/Window.hpp"
#include "graphics/engine/OpenGLEngine.hpp"
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

	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, key_callback);

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
   		this->m_engine->render(this->m_renderable, glfwGetTime(), (float)WINDOW_WIDTH/(float) WINDOW_HEIGHT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    cleanGlfw();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    	win->m_engine->toggleWireframe();
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        win->m_engine->toggleTexture();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        win->m_engine->toggleRotation();
    }
}

void Window::processInput() {
	if(glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->m_window, true);
	if (glfwGetKey(this->m_window, GLFW_KEY_W) == GLFW_PRESS)
		this->m_engine->toggleWireframe();
	if (glfwGetKey(this->m_window, GLFW_KEY_T) == GLFW_PRESS)
		this->m_engine->toggleTexture();
	if (glfwGetKey(this->m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->m_engine->toggleRotation();
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

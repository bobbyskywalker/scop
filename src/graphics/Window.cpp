#include "glad/glad.h"
#include "graphics/Window.hpp"
#include "graphics/engine/Engine.hpp"
#include "graphics/engine/OpenGLEngine.hpp"
#include "GLFW/glfw3.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>

Window::Window(Object3d& renderable) :
	m_renderable(renderable),
	m_deltaTime(0.0f),
	m_currentWindowWidth(DEFAULT_WINDOW_WIDTH),
	m_currentWindowHeight(DEFAULT_WINDOW_HEIGHT)
 {
	if (!glfwInit()) {
		std::cerr << "Error: GLFW initialization failure" << std::endl;
		std::exit(1);
	}
	glfwSetErrorCallback(Window::error_callback);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	this->m_window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, "scop", NULL, NULL);
	if (!this->m_window) {
		std::cerr << "Error: GLFW window creation failure" << std::endl;
		std::exit(1);
	}

	glfwMakeContextCurrent(this->m_window);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: GLAD initialization failure" << std::endl;
        std::exit(1);
    }
	glViewport(0, 0, m_currentWindowWidth, m_currentWindowHeight);
	this->m_engine = std::make_unique<OpenGlEngine>(OpenGlEngine(renderable));
}

void Window::run() {
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        const double currentTime = glfwGetTime();
        m_deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processMovementInput();

        float aspect = 1.0f;
        if (m_currentWindowHeight > 0) {
       		aspect = static_cast<float>(m_currentWindowWidth) / static_cast<float>(m_currentWindowHeight);
        }

        this->m_engine->render(
            this->m_renderable,
            m_deltaTime,
            aspect
        );

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
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
    	win->m_engine->toggleWireframe();
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        win->m_engine->toggleTexture();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        win->m_engine->toggleRotation();
    }
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    win->m_currentWindowWidth = width;
    win->m_currentWindowHeight = height;

    glViewport(0, 0, width, height);
}

void Window::processMovementInput() {
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::LEFT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::RIGHT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::UP, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::DOWN, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::FORWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_engine->updatePos(Engine::MovementDirection::BACKWARD, m_deltaTime);
}

void Window::error_callback(int error, const char* description) {
	std::cerr << "GLFW error: " << "[" << error << "] " << description << std::endl;
	std::exit(1);
}

void Window::cleanGlfw() {
    glfwDestroyWindow(this->m_window);
    glfwTerminate();
}

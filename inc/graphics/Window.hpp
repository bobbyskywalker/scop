#pragma once

#include <GLFW/glfw3.h>
#include "model/Object3d.hpp"
#include "graphics/engine/Engine.hpp"

#include <memory>

constexpr unsigned int DEFAULT_WINDOW_WIDTH = 640;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 480;

class Window {
public:
	explicit Window(Object3d& renderable);

	void initEngine(Object3d& renderable);
	void run();

	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	[[nodiscard]] GLFWwindow* getWindow() { return this->m_window; }

private:
	GLFWwindow* 	                m_window;
	Object3d& 		                m_renderable;
	std::unique_ptr<Engine> 		m_engine;
	float       	                m_deltaTime;
	unsigned int 	                m_currentWindowWidth;
	unsigned int 	                m_currentWindowHeight;

	void cleanGlfw();
	void processMovementInput();
};

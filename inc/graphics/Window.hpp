#pragma once

#include <GLFW/glfw3.h>
#include "../model/Object3d.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define BASE_SHADER_LOCATION "./src/graphics/shaders/"

enum class ShaderLoadable {
    BASIC_VERT,
    BASIC_FRAG
};

class Window {
public:
	Window();
	~Window();

	void initEngine(Object3d& renderable);
	void run(Object3d& renderable);

	static void error_callback(int error, const char* description);

	GLFWwindow* getWindow() { return this->m_window; }

private:
	GLFWwindow* m_window;

	void cleanGlfw();
	void processInput();
	unsigned int compileShader(ShaderLoadable shaderFile, int shaderType);
	std::string loadShader( ShaderLoadable shader);

};

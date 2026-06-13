#include "../../inc/glad/glad.h"
#include "GLFW/glfw3.h"
#include "../../inc/graphics/Window.hpp"
#include "../../inc/exception/MissingShaderFileException.hpp"
#include <GL/glext.h>
#include <cstddef>
#include <iostream>
#include <fstream>

const std::string getShaderFilename(ShaderLoadable shader) {
    if (shader == ShaderLoadable::BASIC_VERT) {
        return std::string(BASE_SHADER_LOCATION) + "basic_vert.glsl";
    }
    return "";
}

// todo: window resizing

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
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

Window::~Window() {}

void Window::run(Object3d renderable) {
	while (!glfwWindowShouldClose(m_window)) {
		// step 1: vertex input
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(renderable.getVertices()), renderable.getVertices().data(), GL_STATIC_DRAW);

		// step 2: vertex shader
		unsigned int vertexShader;
		std::string shaderSrcStr;
		const char *shaderSrc;
		try {
		    shaderSrcStr = loadShader(ShaderLoadable::BASIC_VERT);
			shaderSrc = shaderSrcStr.c_str();
		} catch (const std::exception& e) {
		    std::cerr << "Error: failed to load shader source. Reason: " << e.what() << std::endl;
			break;
		}
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &shaderSrc, NULL);
		glCompileShader(vertexShader);

		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		/* ^^^ test - bg color render */

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
}

std::string Window::loadShader(ShaderLoadable shader) {
    auto path = getShaderFilename(shader);
    std::string line,content;
    std::ifstream in(path);

    if (!in.is_open()) {
        throw MissingShaderFileException("Error: could not open shader file on path: " + path + "\n");
    }

    while(std::getline(in, line)) {
        content += line + "\n";
    }
    return content;
}

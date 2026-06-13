#include "../../inc/glad/glad.h"
#include "GLFW/glfw3.h"
#include "../../inc/graphics/Window.hpp"
#include "../../inc/exception/MissingShaderFileException.hpp"
#include "../../inc/exception/ShaderCompilationException.hpp"
#include "../../inc/exception/ShaderLinkingErrorException.hpp"
#include <GL/glext.h>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <iostream>
#include <fstream>

const std::string getShaderFilename(ShaderLoadable shader) {
    if (shader == ShaderLoadable::BASIC_VERT) {
        return std::string(BASE_SHADER_LOCATION) + "basic_vert.glsl";
    } else if (shader == ShaderLoadable::BASIC_FRAG) {
        return std::string(BASE_SHADER_LOCATION) + "basic_frag.glsl";
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

void Window::initEngine(Object3d& renderable) {
    /* vertex buffer init */
    unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(renderable.getVertices()), renderable.getVertices().data(), GL_STATIC_DRAW);

	/* shader compilation */
	unsigned int vertexShader;
	unsigned int fragmentShader;
	vertexShader = compileShader(ShaderLoadable::BASIC_VERT, GL_VERTEX_SHADER);
	fragmentShader = compileShader(ShaderLoadable::BASIC_FRAG, GL_FRAGMENT_SHADER);

	/* shader linking */
	linkShaders(vertexShader, fragmentShader);
}

void Window::run(Object3d& renderable) {
    (void) renderable;
	while (!glfwWindowShouldClose(m_window)) {
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

unsigned int Window::compileShader(ShaderLoadable shaderFile, int shaderMacro) {
    unsigned int shader;
	std::string shaderSrcStr;
	const char *shaderSrc;

	shaderSrcStr = loadShader(shaderFile);
	shaderSrc = shaderSrcStr.c_str();
	shader = glCreateShader(shaderMacro);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw ShaderCompilationException("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
	}
	return shader;
}

/* accepts a variable number of compiled shaders */
void Window::linkShaders(unsigned int shader, ...) {
    auto shaderProgram = glCreateProgram();

    va_list args;
    va_start(args, shader);
    glAttachShader(shaderProgram, shader);
    while (true) {
        GLuint shader = va_arg(args, GLuint);
        if (shader == 0) break;
        glAttachShader(shaderProgram, shader);
    }
    va_end(args);

    glLinkProgram(shaderProgram);

    int  success;
	char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        throw ShaderLinkingErrorException("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
}

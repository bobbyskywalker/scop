#include "../../inc/glad/glad.h"
#include "../../inc/graphics/Window.hpp"
#include "../../inc/math/math3d.h"
#include "../../inc/exception/MissingShaderFileException.hpp"
#include "../../inc/exception/ShaderCompilationException.hpp"
#include "../../inc/exception/ShaderLinkingErrorException.hpp"
#include "GLFW/glfw3.h"
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
	/* compile shaders */
    unsigned int vertexShader = compileShader(ShaderLoadable::BASIC_VERT, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(ShaderLoadable::BASIC_FRAG, GL_FRAGMENT_SHADER);

    m_shaderProgram = linkShaders(vertexShader, fragmentShader);
    glUseProgram(m_shaderProgram);

    m_mvpLocation = glGetUniformLocation(m_shaderProgram, "u_mvp");

    /* VAO/VBO setup */
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // In Window::initEngine, flatten the vertices
    std::vector<float> vertexData;
    for (const auto& v : renderable.getVertices()) {
        vertexData.push_back(v.x);
        vertexData.push_back(v.y);
        vertexData.push_back(v.z);
    }

    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(float),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    /* link vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* cleanup */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glBindVertexArray(0);
}

void Window::run(Object3d& renderable) {
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    mat4 proj = perspective(45.0f, aspect, 0.1f, 100.0f);
    mat4 view = translate({0.0f, 0.0f, -5.0f});
    mat4 model = mat_identity();
    mat4 mvp = mat_multiply(proj, mat_multiply(view, model));

    while (!glfwWindowShouldClose(m_window)) {
    // Add before drawing
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader and VAO
        glUseProgram(m_shaderProgram);
        glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, &mvp.matrix[0][0]);
        glBindVertexArray(m_VAO);

        // Draw all triangles
        int vertexCount = renderable.getVertices().size();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

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
unsigned int Window::linkShaders(unsigned int shader, ...) {
    unsigned int shaderProgram = glCreateProgram();

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

    return shaderProgram;
}

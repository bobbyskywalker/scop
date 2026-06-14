#include "../../../inc/graphics/engine/OpenGLEngine.hpp"
#include "../../../inc/exception/MissingShaderFileException.hpp"
#include "../../../inc/exception/ShaderCompilationException.hpp"
#include "../../../inc/exception/ShaderLinkingErrorException.hpp"
#include "../../../inc/glad/glad.h"
#include <fstream>
#include <cstdarg>

const std::string getShaderFilename(ShaderLoadable shader) {
    if (shader == ShaderLoadable::BASIC_VERT) {
        return std::string(BASE_SHADER_LOCATION) + "basic_vert.glsl";
    } else if (shader == ShaderLoadable::BASIC_FRAG) {
        return std::string(BASE_SHADER_LOCATION) + "basic_frag.glsl";
    }
    return "";
}

OpenGlEngine::OpenGlEngine(Object3d& renderable) {
	m_isWireframe = false;

	/* compile shaders */
    unsigned int vertexShader = compileShader(ShaderLoadable::BASIC_VERT, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(ShaderLoadable::BASIC_FRAG, GL_FRAGMENT_SHADER);

    m_shaderProgram = linkShaders(vertexShader, fragmentShader, 0);
    glUseProgram(m_shaderProgram);

    /* VAO/VBO/EBO setup */
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    auto vertices = renderable.getVerticesFlat();
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW
    );

    auto indices = renderable.getIndices();
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    			indices.size() * sizeof(unsigned int),
       			indices.data(),
          		GL_STATIC_DRAW
    );

    /* link vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* cleanup */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glBindVertexArray(0);
}

OpenGlEngine::~OpenGlEngine() {}

void OpenGlEngine::render(Object3d& renderable) {
   	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->m_isWireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);

    glDrawElements(GL_TRIANGLES, renderable.getIndices().size(), GL_UNSIGNED_INT, 0);
}

std::string OpenGlEngine::loadShader(ShaderLoadable shader) {
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

unsigned int OpenGlEngine::compileShader(ShaderLoadable shaderFile, int shaderMacro) {
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
unsigned int OpenGlEngine::linkShaders(unsigned int shader, ...) {
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

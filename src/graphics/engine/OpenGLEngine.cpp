#include "graphics/engine/OpenGLEngine.hpp"
#include "exception/textures/TextureDataLoadingException.hpp"
#include "graphics/engine/Engine.hpp"
#include "graphics/engine/texture.hpp"
#include "exception/shaders/MissingShaderFileException.hpp"
#include "exception/shaders/ShaderCompilationException.hpp"
#include "exception/shaders/ShaderLinkingErrorException.hpp"
#include "graphics/engine/texture.hpp"
#include "glad/glad.h"
#include <cmath>
#include <fstream>
#include <cstdarg>
#include "math/math3d.h"
#define _USE_MATH_DEFINES

const std::string getShaderFilename(const ShaderLoadable shader) {
    if (shader == ShaderLoadable::BASIC_VERT) {
        return std::string(BASE_SHADER_LOCATION) + "basic_vert.glsl";
    } else if (shader == ShaderLoadable::BASIC_FRAG) {
        return std::string(BASE_SHADER_LOCATION) + "basic_frag.glsl";
    }
    return "";
}

OpenGlEngine::OpenGlEngine(const Object3d& renderable) {
	m_isWireframe = false;
	m_isTexture = false;
	m_isRotating = false;
	m_objectPos = {0.0f, 0.0f, 0.0f};
	m_movementSpeed = 5.0f;
	m_rotationAngle = 0.0f;
	m_rotationSpeed = 1.0f;

	glEnable(GL_DEPTH_TEST);

	/* compile shaders */
    unsigned int vertexShader = compileShader(ShaderLoadable::BASIC_VERT, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(ShaderLoadable::BASIC_FRAG, GL_FRAGMENT_SHADER);

    m_shaderProgram = linkShaders(vertexShader, fragmentShader, 0);
    glUseProgram(m_shaderProgram);

    /* set uniform locations */
    m_blendLocation = glGetUniformLocation(m_shaderProgram, BLENDING_LOCATION);
    m_vertexColorLocation = glGetUniformLocation(m_shaderProgram, VERTEX_COLOR_LOCATION);
    m_transformLocation = glGetUniformLocation(m_shaderProgram, TRANSFORM_LOCATION);

    /* VAO/VBO/EBO setup */
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    auto vertices = renderable.getVerticesWithUVMappingArray();
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
    /* position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* uv attribute */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* texture data loading */
    m_textureLocation = glGetUniformLocation(m_shaderProgram,  VERTEX_TEXTURE_LOCATION);
    loadTextures(renderable.getMaterials());

    /* cleanup */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glBindVertexArray(0);
    m_vertexColorLocation = glGetUniformLocation(m_shaderProgram, VERTEX_COLOR_LOCATION);
}

OpenGlEngine::~OpenGlEngine() {}

void OpenGlEngine::render(const Object3d& renderable, const float deltaTime, const float aspect) {
    if (m_isRotating) {
        m_rotationAngle += m_rotationSpeed * deltaTime;
    }

	glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);
    mat4 mvp = buildMvp(deltaTime, aspect);
    glUniformMatrix4fv(m_transformLocation, 1, GL_TRUE, &mvp.matrix[0][0]);

	this->m_isWireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(m_VAO);

    for (const auto& b: renderable.getRenderBatches())
   		renderBatch(renderable,b);
}

void OpenGlEngine::renderBatch(const Object3d& renderable, const RenderBatch& currentBatch) {
    auto mtls = renderable.getMaterials();
    Material& currentMtl = mtls[currentBatch.materialName];

    if (m_isTexture && m_textures.count(currentBatch.materialName)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[currentBatch.materialName]);
        glUniform1i(m_textureLocation, 0);
        applySelectedColorMode(currentMtl.getDiffuseColor(), 1.0f);
    } else {
    	applySelectedColorMode(currentMtl.getDiffuseColor(), 0.0f);
    }

    auto start = currentBatch.triangleIndices[0] * 3 * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, currentBatch.triangleIndices.size() * 3, GL_UNSIGNED_INT, (void*)start);
}

void OpenGlEngine::applySelectedColorMode(const std::vector<float>& diffuseColor, float blendingLevel) {
    glUniform3f(m_vertexColorLocation, diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    glUniform1f(m_blendLocation, blendingLevel);
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

void OpenGlEngine::loadTextures(const std::unordered_map<std::string, Material>& materials) {
	for (const auto& [name, material] : materials) {
		if (!material.getDiffuseMap().empty()) {
			unsigned int textureID = loadTexture(material.getDiffuseMap());
			m_textures[name] = textureID;
		}
	}
}

unsigned int OpenGlEngine::loadTexture(const std::string& path) {
    int width, height, nrChannels;
    unsigned char* data = loadTextureData(&width, &height, &nrChannels, path);
    if (!data) {
   		throw TextureDataLoadingException("ERROR::TEXTURE::PROGRAM::LOADING_FAILED\n");
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    freeTextureData(data);

    return textureID;
}

// TODO:
// rotation over main symmetry axis
// constant definition
// object movement
mat4 OpenGlEngine::buildMvp(const float deltaTime, const float aspect)
{
    (void) deltaTime;
    mat4 proj = perspective(45.0f * M_PI / 180.0f, aspect, 0.1f, 100.0f);
    mat4 view = translate({0.0f, 0.0f, -10.0f});

    mat4 model = translate(m_objectPos);

    if (m_isRotating)
        model = mat_multiply(model, rotateY(m_rotationAngle));

    return mat_multiply(proj, mat_multiply(view, model));
}

void OpenGlEngine::updatePos(const MovementDirection dir, const float deltaTime) {
    switch (dir) {
        case Engine::MovementDirection::LEFT:
            this->m_objectPos.x -= m_movementSpeed * deltaTime;
            break;
        case Engine::MovementDirection::RIGHT:
            this->m_objectPos.x += m_movementSpeed * deltaTime;
            break;
        case Engine::MovementDirection::UP:
            this->m_objectPos.y += m_movementSpeed * deltaTime;
            break;
        case Engine::MovementDirection::DOWN:
            this->m_objectPos.y -= m_movementSpeed * deltaTime;
            break;
        case Engine::MovementDirection::FORWARD:
            this->m_objectPos.z += m_movementSpeed * deltaTime;
            break;
        case Engine::MovementDirection::BACKWARD:
            this->m_objectPos.z -= m_movementSpeed * deltaTime;
            break;
    }
}

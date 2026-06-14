#pragma once

#include <string>
#include "../../../inc/model/Object3d.hpp"
#include "Engine.hpp"

#define BASE_SHADER_LOCATION "./src/graphics/shaders/"

enum class ShaderLoadable {
    BASIC_VERT,
    BASIC_FRAG
};

class OpenGlEngine: public Engine {
public:
	OpenGlEngine(Object3d &renderable);
	~OpenGlEngine() override;

	void render(Object3d& renderable) override;
	void toggleWireframe() override {this->m_isWireframe = !this->m_isWireframe;}

private:
	unsigned int	m_shaderProgram;
	unsigned int	m_VAO;
	unsigned int	m_VBO;
	unsigned int	m_EBO;
	bool			m_isWireframe;

	unsigned int 	linkShaders(unsigned int shaders...);
	unsigned int 	compileShader(ShaderLoadable shaderFile, int shaderType);
	std::string 	loadShader(ShaderLoadable shader);
};

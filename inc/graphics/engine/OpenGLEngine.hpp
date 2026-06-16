#pragma once

#include <string>
#include "model/Object3d.hpp"
#include "Engine.hpp"

#define BASE_SHADER_LOCATION "./src/graphics/shaders/"
#define VERTEX_COLOR_LOCATION "m_color"

constexpr float BACKGROUND_COLOR[] = {0.1f, 0.1f, 0.15f, 1.0f};

enum class ShaderLoadable {
    BASIC_VERT,
    BASIC_FRAG
};

class OpenGlEngine: public Engine {
public:
	OpenGlEngine(const Object3d &renderable);
	~OpenGlEngine() override;

	void render(const Object3d& renderable) override;
	void toggleWireframe() override {this->m_isWireframe = !this->m_isWireframe;}
	void toggleTexture() override {return;}

private:
	unsigned int	m_shaderProgram;
	unsigned int	m_VAO;
	unsigned int	m_VBO;
	unsigned int	m_EBO;
	int				m_vertexColorLocation;
	bool			m_isWireframe;

	unsigned int 	linkShaders(unsigned int shaders...);
	unsigned int 	compileShader(const ShaderLoadable shaderFile, const int shaderType);
	std::string 	loadShader(const ShaderLoadable shader);
	void 			renderBatch(const Object3d& renderable, const RenderBatch& currentBatch);
};

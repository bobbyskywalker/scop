#pragma once

#include <string>
#include <unordered_map>
#include "math/math3d.h"
#include "model/Object3d.hpp"
#include "Engine.hpp"

#define BASE_SHADER_LOCATION "./src/graphics/shaders/"
#define VERTEX_COLOR_LOCATION "u_color"
#define VERTEX_TEXTURE_LOCATION "m_texture"
#define BLENDING_LOCATION "u_blend"
#define TRANSFORM_LOCATION "transform"

constexpr float BACKGROUND_COLOR[] = {0.1f, 0.1f, 0.15f, 1.0f};

enum class ShaderLoadable {
    BASIC_VERT,
    BASIC_FRAG
};

class OpenGlEngine: public Engine {
public:
	OpenGlEngine(const Object3d &renderable);
	~OpenGlEngine() override;

	void render(const Object3d& renderable, const float deltaTime, const float aspect) override;
	void toggleWireframe() override {this->m_isWireframe = !this->m_isWireframe;}
	void toggleTexture() override {this->m_isTexture = !this->m_isTexture;}
	void toggleRotation() override {this->m_isRotating = !this->m_isRotating;}

private:
	unsigned int									m_shaderProgram;
	unsigned int									m_VAO;
	unsigned int									m_VBO;
	unsigned int									m_EBO;
	int												m_vertexColorLocation;
	int												m_blendLocation;
	int												m_transformLocation;
	int												m_textureLocation;
	bool											m_isWireframe;
	bool											m_isTexture;
	bool											m_isRotating;
	unsigned char 									*m_diffuseTextureData;
	std::unordered_map<std::string, unsigned int>    m_textures;

	void			loadTextures(const std::unordered_map<std::string, Material>& materials);
	unsigned int 	loadTexture(const std::string& path);
	unsigned int 	linkShaders(unsigned int shaders...);
	unsigned int 	compileShader(const ShaderLoadable shaderFile, const int shaderType);
	std::string 	loadShader(const ShaderLoadable shader);
	void 			renderBatch(const Object3d& renderable, const RenderBatch& currentBatch);
	void 			applySelectedColorMode(const std::vector<float>& diffuseColor, float blendingLevel);
	mat4			buildMvp(const float deltaTime, const float aspect);
};

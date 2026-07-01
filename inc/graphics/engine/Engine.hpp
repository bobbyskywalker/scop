#pragma once

#include "model/Object3d.hpp"

class Engine {
public:
	virtual ~Engine() = default;
	virtual void render(const Object3d& renderable, const float deltaTime, const float aspect) = 0;
	virtual void toggleWireframe() = 0;
	virtual void toggleTexture() = 0;
	virtual void toggleRotation() = 0;
};

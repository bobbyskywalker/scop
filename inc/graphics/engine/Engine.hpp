#pragma once

#include "model/Object3d.hpp"

class Engine {
public:
    enum class MovementDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        FORWARD,
        BACKWARD
    };
	virtual ~Engine() = default;
	virtual void render(const Object3d& renderable, const float deltaTime, const float aspect) = 0;
	virtual void toggleWireframe() = 0;
	virtual void toggleTexture() = 0;
	virtual void toggleRotation() = 0;
	virtual void updatePos(const MovementDirection dir, const float deltaTime) = 0;
};

#pragma once

#include "../../../inc/model/Object3d.hpp"

class Engine {
public:
	virtual ~Engine() = default;
	virtual void render(Object3d& renderable) = 0;
	virtual void toggleWireframe() = 0;
};

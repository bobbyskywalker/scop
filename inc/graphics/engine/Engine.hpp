#pragma once

#include "../../../inc/model/Object3d.hpp"
#include "../../../inc/math/math3d.h"

class Engine {
public:
	virtual ~Engine() = default;
	virtual void render(Object3d& renderable, const mat4& mvp) = 0;
};

#pragma once

#include <string>
#include <vector>
#include "math/math3d.h"
# define PARSER_ERROR_LOG "Exception occurred when parsing line "

std::vector<std::string> split(const std::string& s, char delim);
vec3 toVec3(const std::vector<float>& vec);
vec3 negate(const vec3& vec);

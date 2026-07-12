#include "util.hpp"
#include "math/math3d.h"
#include <sstream>

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> res;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) res.push_back(item);
    }
    return res;
}

vec3 toVec3(const std::vector<float>& vec) {
    if (vec.size() < 3) {
        throw std::invalid_argument("Invalid vector size");
    }
    return {vec[0],vec[1],vec[2]};
}

vec3 negate(const vec3& vec) {
    return {-vec.x,-vec.y,-vec.z};
}

#pragma once

#include <vector>

class Material {
public:
    Material() {}
    ~Material() {}
private:
    std::string         textureMap;
    std::vector<float>  diffuseColor;
};

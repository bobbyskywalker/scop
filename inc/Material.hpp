#pragma once

#include <vector>

class Material {
public:
    Material(): diffuseMap(""), diffuseColor({}){}
    ~Material() {}

    void setDiffuseMap(std::string diffuseMap) {
        this->diffuseMap = diffuseMap;
    }

    void setDiffuseColor(std::vector<float> diffuseColor) {
        this->diffuseColor = diffuseColor;
    }

private:
    std::string         diffuseMap;
    std::vector<float>  diffuseColor;
};

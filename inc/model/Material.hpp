#pragma once

#include <vector>
#include <string>

class Material {
public:
    Material(): diffuseMap(""), diffuseColor({}){}
    ~Material() {}

    std::string getDiffuseMap() { return this->diffuseMap; }
    void setDiffuseMap(std::string diffuseMap) { this->diffuseMap = diffuseMap; }

    std::vector<float> getDiffuseColor() { return this->diffuseColor; };
    void setDiffuseColor(std::vector<float> diffuseColor) { this->diffuseColor = diffuseColor; }

private:
    std::string         diffuseMap;
    std::vector<float>  diffuseColor;
};

#pragma once

#include <vector>
#include <string>

#define DEFAULT_MTL_ID "default"
constexpr float DEFAULT_MTL_COLOR[] = {0.5f, 0.5f, 0.5f};

class Material {
public:
    Material(): diffuseMap(""), diffuseColor({0.5f, 0.5f, 0.5f}) {}
    ~Material() {}

    std::string getDiffuseMap() const { return this->diffuseMap; }
    void setDiffuseMap(const std::string diffuseMap) { this->diffuseMap = diffuseMap; }

    std::vector<float> getDiffuseColor() const { return this->diffuseColor; };
    void setDiffuseColor(const std::vector<float> diffuseColor) { this->diffuseColor = diffuseColor; }

private:
    std::string         diffuseMap;
    std::vector<float>  diffuseColor;
};

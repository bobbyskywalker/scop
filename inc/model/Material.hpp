#pragma once

#include <vector>
#include <string>

static constexpr const char* DEFAULT_MTL_ID = "default";
constexpr float DEFAULT_MTL_COLOR[] = {0.5f, 0.5f, 0.5f};

class Material {
public:
    explicit Material(): diffuseMap(""), diffuseColor({0.5f, 0.5f, 0.5f}) {}

    [[nodiscard]] std::string           getDiffuseMap() const { return this->diffuseMap; }
    void                                setDiffuseMap(const std::string diffuseMap) { this->diffuseMap = diffuseMap; }

    [[nodiscard]] std::vector<float>    getDiffuseColor() const { return this->diffuseColor; };
    void                                setDiffuseColor(const std::vector<float> diffuseColor) { this->diffuseColor = diffuseColor; }

private:
    std::string         diffuseMap;
    std::vector<float>  diffuseColor;
};

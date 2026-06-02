#pragma once

#include "Material.hpp"
#include <map>
#include <string>

class MaterialLoader {
public:
    static std::map<std::string, Material> parseMaterials(const std::string& filename);
};

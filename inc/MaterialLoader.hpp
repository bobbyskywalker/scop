#pragma once

#include "Material.hpp"
#include <string>
#include <unordered_map>
#include <vector>

enum class MtlType {
    NEW_MATERIAL,

    AMBIENT_COLOR,
    DIFFUSE_COLOR,
    SPECULAR_COLOR,
    EMISSIVE_COLOR,

    SPECULAR_EXPONENT,
    OPTICAL_DENSITY,
    DISSOLVE,
    TRANSPARENCY,
    ILLUMINATION,

    AMBIENT_MAP,
    DIFFUSE_MAP,
    SPECULAR_MAP,
    SHININESS_MAP,
    ALPHA_MAP,
    BUMP_MAP,
    BUMP_MAP_ALT,
    DISP_MAP,
    DECAL_MAP,
    REFLECTION_MAP,

    COMMENT,
    UNKNOWN
};

MtlType getMtlTokenType(const std::string& token);

class MaterialLoader {
public:
    static std::unordered_map<std::string, Material> parseMaterials(const std::string& filename);

private:
    static void logInfo(const std::string& msg);
    static void logError(int lineIdx, const std::string& msg);

    static bool parseLine(
        const int lineIdx,
        const std::string& line,
        std::unordered_map<std::string, Material>& materials,
        std::string& currentName,
        Material& currentMaterial
    );

    static void parseColor(
        const int lineIdx,
        const std::vector<std::string>& tokens,
        Material& currentMaterial
    );

};

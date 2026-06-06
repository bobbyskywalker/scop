#include "../inc/model/MaterialLoader.hpp"
#include "../inc/util.hpp"
#include "../inc/exception/UnknownKeyInObjectFileException.hpp"
#include "../inc/exception/InvalidColorParamsException.hpp"
#include "../inc/exception/FileUnprocessableException.hpp"
#include "../inc/exception/MalformedMaterialFileDeclarationException.hpp"
#include <exception>
#include <fstream>
#include <iostream>

MtlType getMtlTokenType(const std::string& token) {
    if (token.empty())
        return MtlType::UNKNOWN;
    if (token[0] == '#')      return MtlType::COMMENT;
    if (token == "newmtl")    return MtlType::NEW_MATERIAL;
    if (token == "Ka")        return MtlType::AMBIENT_COLOR;
    if (token == "Kd")        return MtlType::DIFFUSE_COLOR;
    if (token == "Ks")        return MtlType::SPECULAR_COLOR;
    if (token == "Ke")        return MtlType::EMISSIVE_COLOR;
    if (token == "Ns")        return MtlType::SPECULAR_EXPONENT;
    if (token == "Ni")        return MtlType::OPTICAL_DENSITY;
    if (token == "d")         return MtlType::DISSOLVE;
    if (token == "Tr")        return MtlType::TRANSPARENCY;
    if (token == "illum")     return MtlType::ILLUMINATION;
    if (token == "map_Ka")    return MtlType::AMBIENT_MAP;
    if (token == "map_Kd")    return MtlType::DIFFUSE_MAP;
    if (token == "map_Ks")    return MtlType::SPECULAR_MAP;
    if (token == "map_Ns")    return MtlType::SHININESS_MAP;
    if (token == "map_d")     return MtlType::ALPHA_MAP;
    if (token == "bump")      return MtlType::BUMP_MAP;
    if (token == "map_Bump")  return MtlType::BUMP_MAP_ALT;
    if (token == "disp")      return MtlType::DISP_MAP;
    if (token == "decal")     return MtlType::DECAL_MAP;
    if (token == "refl")      return MtlType::REFLECTION_MAP;
    return MtlType::UNKNOWN;
}

void MaterialLoader::logInfo(const std::string& msg) {
	static std::ofstream logFile("debug_mtl.log");
    logFile << msg << std::endl;
}

void MaterialLoader::logError(int lineIdx, const std::string& errmsg) {
	static std::ofstream logFile("debug_mtl.log");
	logFile << PARSER_ERROR_LOG + std::to_string(lineIdx) << ": " << errmsg;
}

std::unordered_map<std::string, Material>
MaterialLoader::parseMaterials(const std::string &filename) {
    int idx = 1;
    std::string line;
    std::ifstream mtlFile(filename);

    std::unordered_map<std::string, Material> materials;

    std::string currentName;
    Material currentMaterial;

    if (mtlFile.is_open()) {
        while (std::getline(mtlFile, line)) {
            if (!parseLine(idx++, line, materials, currentName, currentMaterial)) {
                mtlFile.close();
                throw MalformedMaterialFileDeclarationException("Error: Unable to parse material file.");
            }
        }
        mtlFile.close();
    }
    else {
        logError(0, "Failed to open file.");
        throw FileUnprocessableException("Failed to open material library file.");
    }

    if (!currentName.empty())
        materials[currentName] = currentMaterial;

    return materials;
}

bool MaterialLoader::parseLine(
    const int lineIdx,
    const std::string& line,
    std::unordered_map<std::string, Material>& materials,
    std::string& currentName,
    Material& currentMaterial
) {
    try {
        if (!line.empty()) {
            std::vector<std::string> tokens = split(line, ' ');
            MtlType type = getMtlTokenType(tokens.at(0));

            switch (type) {
                case MtlType::NEW_MATERIAL:
                    if (!currentName.empty())
                        materials[currentName] = currentMaterial;

                    currentName = tokens.at(1);
                    currentMaterial = Material();
                    logInfo("Starting to parse material named: " + currentName);
                    break;

                case MtlType::DIFFUSE_COLOR:
                    parseColor(tokens, currentMaterial);
                    logInfo("Successfuly parsed diffuse color at line " + std::to_string(lineIdx) + "\n");
                    break;

                case MtlType::DIFFUSE_MAP:
                    currentMaterial.setDiffuseMap(tokens.at(1));
                    logInfo("Successfuly diffuse texture map at line " + std::to_string(lineIdx) + "\n");
                    break;

                case MtlType::UNKNOWN:
                    throw UnknownKeyInObjectFileException("Unknown key: " + tokens.at(0));
                default:
                    break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[Line " << lineIdx << "]: " << e.what() << std::endl;
        logError(lineIdx, e.what());
        return false;
    }
    return true;
}

void MaterialLoader::parseColor(
    const std::vector<std::string>& tokens,
    Material& currentMaterial
) {
    if (tokens.size() != 4) {
        throw InvalidColorParamsException("Invalid color definition. Invalid number of arguments.");
    }
    auto r = std::stof(tokens[1]);
    auto g = std::stof(tokens[2]);
    auto b = std::stof(tokens[3]);
    currentMaterial.setDiffuseColor(std::vector<float>{r, g, b});
}

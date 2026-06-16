#include "../../inc/model/Object3d.hpp"
#include "../../inc/model/MaterialLoader.hpp"
#include "../../inc/util.hpp"
#include "../../inc/exception/InvalidVertexParamsException.hpp"
#include "../../inc/exception/InvalidFaceParamsException.hpp"
#include "../../inc/exception/InvalidTexCoordParamsException.hpp"
#include "../../inc/exception/InvalidNormalParamsException.hpp"
#include "../../inc/exception/UnknownKeyInObjectFileException.hpp"
#include "../../inc/exception/InvalidMtlLibParamsException.hpp"
#include "../../inc/exception/InvalidUseMtlDirective.hpp"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <algorithm>

ObjType getObjTokenType(const std::string& token) {
    if (token == "v") return ObjType::VERTEX;
    if (token == "vt") return ObjType::TEXCOORD;
    if (token == "vn") return ObjType::NORMAL;
    if (token == "f") return ObjType::FACE;
    if (token == "o") return ObjType::NAME;
    if (token == "mtllib") return ObjType::MTL_LIB;
    if (token == "usemtl") return ObjType::USE_MTL;
    if (token == "s") return ObjType::SMOOTHING_GROUP;
    if (token[0] == '#') return ObjType::COMMENT;
    return ObjType::UNKNOWN;
}

Object3d::Object3d(const std::string& filename) : m_currentMaterialID("") {
	loadObjFromFile(filename);
}

Object3d::~Object3d() {}

void Object3d::logInfo(const std::string& msg) {
	static std::ofstream logFile("debug_obj.log");
    logFile << msg << std::endl;
}

void Object3d::logError(int lineIdx, const std::string& errmsg) {
	static std::ofstream logFile("debug_obj.log");
	logFile << PARSER_ERROR_LOG + std::to_string(lineIdx) << ": " << errmsg;
}

void Object3d::loadObjFromFile(const std::string& filename) {
	int idx = 1;
	std::string line;
	std::ifstream objFile(filename);

	if (objFile.is_open()) {
		while(std::getline(objFile, line)) {
			if (!parseLine(idx++, line)) {
				break;
			}
		}
		objFile.close();
	} else {
		std::cerr << "Error: failed to open file " << filename << std::endl;
		logError(0, "Failed to open file.");
	}
	buildRenderBatches();
}

bool Object3d::parseLine(const int lineIdx, const std::string& line) {
	try {
		if (!line.empty()) {
			std::vector<std::string> tokens = split(line, ' ');
			switch (getObjTokenType(tokens.at(0))) {
				case ObjType::VERTEX:
					parseVertex(tokens);
					logInfo("Successfully parsed vertex at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::TEXCOORD:
					parseTexCoord(tokens);
					logInfo("Successfully parsed texture coord at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::NORMAL:
					parseNormal(tokens);
					logInfo("Successfully normal at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::FACE:
					parseFace(tokens);
					logInfo("Successfully parsed face at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::COMMENT:
					break;
				case ObjType::MTL_LIB:
                    parseMaterials(tokens);
                    logInfo("Successfully parsed material library at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::USE_MTL:
					parseAndSetCurrentMaterial(tokens);
					logInfo("Successfully parsed a usemtl directive at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::SMOOTHING_GROUP:
					break;
				case ObjType::UNKNOWN:
					throw UnknownKeyInObjectFileException("Unknown key: " + tokens.at(0));
					break;
				default:
					break;
			}
		}
	} catch (const std::exception& e) {
	    std::cerr << "ERROR [Line " << lineIdx << "]: " << e.what() << std::endl;
	    logError(lineIdx, e.what());
	    return false;
	}
	return true;
}

void Object3d::parseVertex(const std::vector<std::string>& tokens) {
	if (tokens.size() != 4) {
		throw InvalidVertexParamsException("Invalid vertex definition: invalid number of arguments.");
	}
	auto v = Vertex(
		std::stof(tokens[1]),
		std::stof(tokens[2]),
		std::stof(tokens[3])
	);
	this->m_vertices.push_back(v);
}

void Object3d::parseTexCoord(const std::vector<std::string>& tokens) {
	if (tokens.size() != 3) {
		throw InvalidTexCoordParamsException("Invalid texture definition: invalid number of arguments.");
	}
	auto tx = TexCoord(
		std::stof(tokens[1]),
		std::stof(tokens[2])
	);
	this->m_texcoords.push_back(tx);
}

void Object3d::parseNormal(const std::vector<std::string>& tokens) {
	if (tokens.size() != 4 ) {
		throw InvalidNormalParamsException("Invalid normal definition: invalid number of arguments.");
	}
	auto n = Normal(
		std::stof(tokens[1]),
		std::stof(tokens[2]),
		std::stof(tokens[3])
	);
	this->m_normals.push_back(n);
}

/* The .obj file indexing starts from 1, which is super fucking dumb :)
 * The tokens are passed to the utility with an off-by-one guard.
 */
void Object3d::parseFace(const std::vector<std::string>& tokens) {
    if (tokens.size() == 4) {
        Triangle triangle = Triangle{};
        for (int i = 0; i < 3; i++) {
            parseFaceTokenIndices(triangle, i, tokens[i + 1]);
        }
        triangle.materialId = m_currentMaterialID;
        m_triangles.push_back(triangle);

    } else if (tokens.size() == 5) {
        Quad quad = Quad{};
        for (int i = 0; i < 4; i++) {
            parseFaceTokenIndices(quad, i, tokens[i + 1]);
        }
        Triangle t1, t2;
        triangulateQuad(quad, t1, t2);
        t1.materialId = m_currentMaterialID;
        m_triangles.push_back(t1);
        t2.materialId = m_currentMaterialID;
        m_triangles.push_back(t2);
    } else {
        throw InvalidFaceParamsException("Invalid face definition: invalid number of arguments.");
    }
}

template<typename T>
void Object3d::parseFaceTokenIndices(T& shape, int pos, const std::string& indicesStr) {
    std::vector<std::string> indices = split(indicesStr, FACE_TOKEN_DELIMITER);

    shape.verticesId[pos] = std::stoi(indices[0]) - 1;

    if (indices.size() >= 2 && !indices[1].empty()) {
        shape.texCordIdx[pos] = std::stoi(indices[1]) - 1;
    } else {
        shape.texCordIdx[pos] = -1;
    }

    if (indices.size() >= 3 && !indices[2].empty()) {
        shape.normalIdx[pos] = std::stoi(indices[2]) - 1;
    } else {
        shape.normalIdx[pos] = -1;
    }
}

void Object3d::triangulateQuad(Quad& q, Triangle& t1, Triangle& t2) {
    t1.verticesId[0] = q.verticesId[0];
    t1.verticesId[1] = q.verticesId[1];
    t1.verticesId[2] = q.verticesId[2];

    t2.verticesId[0] = q.verticesId[0];
    t2.verticesId[1] = q.verticesId[2];
    t2.verticesId[2] = q.verticesId[3];

    if (q.texCordIdx[0] != -1) {
        t1.texCordIdx[0] = q.texCordIdx[0];
        t1.texCordIdx[1] = q.texCordIdx[1];
        t1.texCordIdx[2] = q.texCordIdx[2];

        t2.texCordIdx[0] = q.texCordIdx[0];
        t2.texCordIdx[1] = q.texCordIdx[2];
        t2.texCordIdx[2] = q.texCordIdx[3];
    }

    if (q.normalIdx[0] != -1) {
        t1.normalIdx[0] = q.normalIdx[0];
        t1.normalIdx[1] = q.normalIdx[1];
        t1.normalIdx[2] = q.normalIdx[2];

        t2.normalIdx[0] = q.normalIdx[0];
        t2.normalIdx[1] = q.normalIdx[2];
        t2.normalIdx[2] = q.normalIdx[3];
    }
}

void Object3d::parseMaterials(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        throw InvalidMtlLibParamsException("Invalid material library definition. Invalid number of arguments.");
    }
    auto materials = MaterialLoader::parseMaterials(tokens.at(1));
    m_materials.insert(materials.begin(), materials.end());
}

void Object3d::parseAndSetCurrentMaterial(const std::vector<std::string>& tokens) {
	if (tokens.size() != 2) {
		throw InvalidUseMtlDirective("Invalid usemtl directive. Invalid number of arguments.");
	}
	this->m_currentMaterialID = tokens.at(1);
}

void Object3d::buildRenderBatches() {
	std::sort(this->m_triangles.begin(), this->m_triangles.end(), [](const Triangle& a, const Triangle& b){
		return a.materialId < b.materialId;
	});

    std::unordered_map<std::string, std::vector<int>> batchMap;

    for (size_t i = 0; i < m_triangles.size(); i++) {
        std::string matId = m_triangles[i].materialId;
        if (matId.empty()) {
            matId = DEFAULT_MTL_ID;
        }
        batchMap[matId].push_back(i);
    }

    m_renderBatches.clear();
    for (auto& [name, indices] : batchMap) {
        RenderBatch batch(name);
        batch.triangleIndices = indices;
        m_renderBatches.push_back(batch);
    }
}

void Object3d::printObject() {
    for (std::size_t i = 0; i < m_vertices.size(); i++) {
        std::cout << "Vertex " << i+1 << ": ("
                  << m_vertices[i].x << ", "
                  << m_vertices[i].y << ", "
                  << m_vertices[i].z << ")" << std::endl;
    }

    for (std::size_t i = 0; i < m_texcoords.size(); i++) {
        std::cout << "TexCoord " << i+1 << ": ("
                  << m_texcoords[i].u << ", "
                  << m_texcoords[i].v << ")" << std::endl;
    }

    for (std::size_t i = 0; i < m_normals.size(); i++) {
        std::cout << "Normal " << i+1 << ": ("
                  << m_normals[i].nx << ", "
                  << m_normals[i].ny << ", "
                  << m_normals[i].nz << ")" << std::endl;
    }

    for (std::size_t i = 0; i < m_triangles.size(); i++) {
        std::cout << "Triangle " << i+1 << ": vertices ["
                  << m_triangles[i].verticesId[0] << ", "
                  << m_triangles[i].verticesId[1] << ", "
                  << m_triangles[i].verticesId[2] << "] texCoords ["
                  << m_triangles[i].texCordIdx[0] << ", "
                  << m_triangles[i].texCordIdx[1] << ", "
                  << m_triangles[i].texCordIdx[2] << "] normals ["
                  << m_triangles[i].normalIdx[0] << ", "
                  << m_triangles[i].normalIdx[1] << ", "
                  << m_triangles[i].normalIdx[2] << "]" << std::endl;
    }
}

#include "../inc/Object3d.hpp"
#include "../inc/util.hpp"
#include "../inc/exception/InvalidVertexParamsException.hpp"
#include "../inc/exception/UnknownKeyInObjectFileException.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

ObjType getType(const std::string& token) {
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

Object3d::Object3d(std::string& filename) {
	loadObjFromFile(filename);
}

Object3d::~Object3d() {}

void Object3d::logInfo(const std::string& msg) {
	static std::ofstream logFile("debug.log");
    logFile << msg << std::endl;
}

void Object3d::logError(int lineIdx, const std::string& errmsg) {
	static std::ofstream logFile("debug.log");
	logFile << PARSER_ERROR_LOG + std::to_string(lineIdx) << ": " << errmsg;
}

void Object3d::loadObjFromFile(std::string& filename) {
	int idx = 1;
	std::string line;
	std::ifstream objFile(filename);

	if (objFile.is_open()) {
		while(std::getline(objFile, line)) {
			if (!parseLine(idx++, line))
				break;
		}
		objFile.close();
	} else {
		std::cerr << "Error: failed to open file " << filename << std::endl;
		logError(0, "Failed to open file.");
	}
}

bool Object3d::parseLine(const int lineIdx, const std::string& line) {
	try {
		if (!line.empty()) {
			std::vector<std::string> tokens = split(line, ' ');
			switch (getType(tokens.at(0))) {
				case ObjType::VERTEX:
					parseVertex(tokens);
					logInfo("Successfully parsed vertex at line " + std::to_string(lineIdx) + "\n");
					break;
				case ObjType::TEXCOORD:
					// parseTexCoord(tokens);
					break;
				case ObjType::NORMAL:
					// parseNormal(tokens);
					break;
				case ObjType::FACE:
					// parseFace(tokens);
					break;
				case ObjType::COMMENT:
					break;
				case ObjType::MTL_LIB:
					break;
				case ObjType::USE_MTL:
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
	    std::cerr << "[Line " << lineIdx << "]: " << e.what() << std::endl;
	    logError(lineIdx, e.what());
	    return false;
	}
	return true;
}

void Object3d::parseVertex(const std::vector<std::string>& tokens) {
	if (tokens.size() != 4) {
		throw InvalidVertexParamsException("Invalid vertex definition: too many arguments.");
	}
	auto v = Vertex(
		std::stof(tokens[1]),
		std::stof(tokens[2]),
		std::stof(tokens[3])
	);
	this->m_vertices.push_back(v);
}

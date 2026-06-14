#pragma once

#include "Material.hpp"
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

# define FACE_TOKEN_DELIMITER '/'

enum class ObjType {
    VERTEX,
    TEXCOORD,
    NORMAL,
    FACE,
    COMMENT,
    MTL_LIB,
    USE_MTL,
    NAME,
    SMOOTHING_GROUP,
    UNKNOWN
};

ObjType getObjTokenType(const std::string& token);

/*
  --- .obj file legend ---
 * v - vertex pos
 * vt - texture coord
 * vn - vertex normal
 * f - face
 * # - comment
*/

/*
o	Obj name
g	Group name
s	Smoothing group
usemtl	Material name
mtllib	Material library */

struct Vertex {
	float x,y,z;
	Vertex(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
};

struct TexCoord {
    float u, v;
    TexCoord(float u=0, float v=0) : u(u), v(v) {}
};

struct Normal {
    float nx, ny, nz;
    Normal(float nx=0, float ny=0, float nz=0) : nx(nx), ny(ny), nz(nz) {}
};

struct Triangle {
	int verticesId[3];
	int texCordIdx[3];
	int normalIdx[3];
	Triangle() {
        for (int i = 0; i < 3; i++) {
            verticesId[i] = -1;
            texCordIdx[i] = -1;
            normalIdx[i] = -1;
        }
    }
};

struct Quad {
	int verticesId[4];
	int texCordIdx[4];
	int normalIdx[4];
	Quad() {
        for (int i = 0; i < 4; i++) {
            verticesId[i] = -1;
            texCordIdx[i] = -1;
            normalIdx[i] = -1;
        }
    }
};

class Object3d {
public:
	Object3d(const std::string& filename);
	~Object3d();

	void loadObjFromFile(const std::string& filename);

	std::vector<Vertex> getVertices() { return this->m_vertices; }

	std::vector<float> getVerticesFlat() {
		std::vector<float> vertexData;
    	for (const auto& v : getVertices()) {
        vertexData.push_back(v.x);
        vertexData.push_back(v.y);
        vertexData.push_back(v.z);
	    }
		return vertexData;
	}

	std::vector<TexCoord> getTexCoords() { return this->m_texcoords; }

	std::vector<Normal> getNormals() { return this->m_normals; }

	std::vector<Triangle> getTriangles() { return this->m_triangles; }

	void printObject();

private:
	std::vector<Vertex>             m_vertices;
    std::vector<TexCoord>           m_texcoords;
    std::vector<Normal>             m_normals;
    std::vector<Triangle>           m_triangles;
    std::unordered_map<std::string, Material> m_materials;

    bool parseLine(const int lineIdx, const std::string& line);
    void parseVertex(const std::vector<std::string>& tokens);
    void parseTexCoord(const std::vector<std::string>& tokens);
    void parseNormal(const std::vector<std::string>& tokens);
    void parseFace(const std::vector<std::string>& tokens);
    template<typename T>
    void parseFaceTokenIndices(T& shape, int pos, const std::string& indicesStr);
    void triangulateQuad(Quad& q, Triangle& t1, Triangle& t2);
    void parseMaterials(const std::vector<std::string>& tokens);

    void logInfo(const std::string& msg);
    void logError(int lineIdx, const std::string& errmsg);

};

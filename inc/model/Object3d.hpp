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
	int 			verticesId[3];
	int 			texCordIdx[3];
	int 			normalIdx[3];
	std::string 	materialId;
	Triangle() : materialId(DEFAULT_MTL_ID) {
        for (int i = 0; i < 3; i++) {
            verticesId[i] = -1;
            texCordIdx[i] = -1;
            normalIdx[i] = -1;
        }
    }
};

struct Quad {
	int 			verticesId[4];
	int 			texCordIdx[4];
	int 			normalIdx[4];
	std::string 	materialId;
	Quad(): materialId(DEFAULT_MTL_ID) {
        for (int i = 0; i < 4; i++) {
            verticesId[i] = -1;
            texCordIdx[i] = -1;
            normalIdx[i] = -1;
        }
    }
};

struct RenderBatch {
    std::string 		materialName;
    std::vector<int> 	triangleIndices;
    int 				startTriangle;
    int 				batchSize;

    RenderBatch() : materialName(DEFAULT_MTL_ID) {}
    RenderBatch(const std::string& name) : materialName(name) {}
};

class Object3d {
public:
	Object3d(const std::string& filename);
	~Object3d();

	void loadObjFromFile(const std::string& filename);

	const std::vector<Vertex> getVertices() const { return this->m_vertices; }

	const std::vector<float> getVerticesFlat() const {
		std::vector<float> vertexData;
    	for (const auto& v : getVertices()) {
	        vertexData.push_back(v.x);
	        vertexData.push_back(v.y);
	        vertexData.push_back(v.z);
	    }
		return vertexData;
	}

	std::vector<TexCoord> getTexCoords() const { return this->m_texcoords; }

	std::vector<Normal> getNormals() const { return this->m_normals; }

	std::vector<Triangle> getTriangles() const { return this->m_triangles; }

	std::vector<unsigned int> getIndices() const {
	    std::vector<unsigned int> idx;
	    for (const auto& tri : m_triangles) {
	        idx.push_back(tri.verticesId[0]);
	        idx.push_back(tri.verticesId[1]);
	        idx.push_back(tri.verticesId[2]);
	    }
	    return idx;
	}

	std::vector<float> getVerticesWithUVMappingArray() const {
		std::vector<float> data;
    	for (size_t i = 0; i < m_vertices.size(); i++) {
     		data.push_back(m_vertices[i].x);
         	data.push_back(m_vertices[i].y);
          	data.push_back(m_vertices[i].z);

           if (i < m_texcoords.size()) {
            	data.push_back(m_texcoords[i].u);
             	data.push_back(-m_texcoords[i].v);
           } else {
            	data.push_back(0.0f);
             	data.push_back(0.0f);
           }
     	}
     	return data;
	}

	std::unordered_map<std::string, Material> getMaterials() const { return this->m_materials; }

	const std::vector<RenderBatch> getRenderBatches() const { return this->m_renderBatches; }

	void printObject() const;

private:
	std::vector<Vertex>             			m_vertices;
    std::vector<TexCoord>           			m_texcoords;
    std::vector<Normal>             			m_normals;
    std::vector<Triangle>           			m_triangles;
    std::string 								m_currentMaterialID;
    std::unordered_map<std::string, Material> 	m_materials;
    std::vector<RenderBatch>					m_renderBatches;

    bool parseLine(const int lineIdx, const std::string& line);
    void parseVertex(const std::vector<std::string>& tokens);
    void parseTexCoord(const std::vector<std::string>& tokens);
    void parseNormal(const std::vector<std::string>& tokens);
    void parseFace(const std::vector<std::string>& tokens);
    void parseAndSetCurrentMaterial(const std::vector<std::string>& tokens);
    template<typename T>
    void parseFaceTokenIndices(T& shape, const int pos, const std::string& indicesStr);
    void triangulateQuad(const Quad& q, Triangle& t1, Triangle& t2);
    void parseMaterials(const std::vector<std::string>& tokens);
    void loadTextures();
    void buildRenderBatches();

    void logInfo(const std::string& msg);
    void logError(const int lineIdx, const std::string& errmsg);

};

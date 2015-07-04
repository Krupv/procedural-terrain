#pragma once
#include "../main.hpp"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex;

typedef struct indexedModel
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    
    void calcNormals();
} IndexedModel;


// class: Mesh
class Mesh {
public:
	Mesh( Vertex* _p_verticies, unsigned int _numVerticies, unsigned int* _indicies, unsigned int _numIndicies, bool _calcNormal = false );
	Mesh(const std::string& fileName);
	Mesh(const IndexedModel& _model);
	~Mesh();

	void draw() const;

	void initMesh(const IndexedModel& _model);

private:
	enum 
	{ 
		E_POSITION_VB = 0, 
		E_TEXCOOR_VB, 
		E_NORMAL_VB,
		E_IBO_VB, 

		E_NUM_BUFFERS	
	};

	GLuint m_VAO;
	GLuint m_VAB[E_NUM_BUFFERS];
	unsigned int m_drawCount;

	void calcNormal(IndexedModel* _model);
}; // end of class



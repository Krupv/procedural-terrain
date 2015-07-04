//#pragma once
//#include "../main.hpp"
//
//#include "../core/Transform.hpp"
//#include "../core/Vertex.hpp"
//#include "../rendering/Mesh.hpp"
//
//#define PERLIN 1
//
//#ifdef PERLIN
//	#include "noises/MyPerlin.hpp"
//#else
//	#include "noises/simplexnoise.hpp"
//#endif
//
//#define  QUADS_IN_TRI 2
//#define  VERT_IN_TRI 3
//#define STRIP_RES 0 
//class Chunk {
//public: 
//	Mesh* m_mesh;
//
//#ifdef PERLIN
//	Chunk(float _offsetX, float _offsetY, int _nQuads, float _size, PerlinNoise* _noise); 
//#else
//	Chunk(float _offsetX, float _offsetY, int _nQuads, float _size, ValueNoise* _noise); 
//#endif
//
//	~Chunk();
//
//	const Transform* getTransform() const
//	{
//		return m_transform;
//	}
//
//	bool renderingRdy;
//	bool dataRdy;
//	void createMesh();
//private:
//
//	Transform* m_transform;
//
//	const float m_offX; //offset from the whole world
//	const float m_offY; //offset --"-- Y
//
//	//dynamic for lod etc...
//	const int m_numQuads; //number of quads in one row (again only one since square)
//	int m_height; //max height of the terrain chunk
//	
//	//TODO: could be static (m_size) (probably just const and generator should have static he will pass)
//	const float m_size; //hight and width of the chunk (right now one number since its a square)
//
//	//TODO: probably move the mesh to a different class with its own methodes divided from chunk class
//	Vertex* m_verticies; //verticies of the terrain mesh
//	unsigned int* m_indicies; //indicies of the terrain mesh
//	IndexedModel m_model;
//	int m_strip;
//	
//	//constant values that will be calculated once and than stored
//	float m_quadSize; //calculated size of the side of one of the quads that is made out of two triangles
//
//#ifdef PERLIN
//	PerlinNoise* m_noise;
//#else 
//	ValueNoise* m_noise;
//#endif // end of PERLIN
//
//
//
//
//	//METHODES
//	//------------------------------
//	float calculateQuadSize() const;
//	void prepareMesh();
//	void prepareMeshOld();
//	void genHeightMap();
//
//};

#pragma once
#include "../main.hpp"

#include "Chunk.hpp"

#define D_CHUNK_SIZE 10.f
#define D_NUM_QUADS 3 
#define D_HEIGHT 10

#define MAX_ARRAY_SIZE 1000
#define TMP 1000

// class Generator {
// public:
// 	//later made by list, generator will put there new chunks and check them to remove old
// 	//draw chunks by passing world coord of camera, which will choose which chunks to draw
// 	Chunk* m_chList[TMP];
// 	int counter;
//
//
// 	//METHODES
// 	//------------------------------
// 	Generator(float _size = D_CHUNK_SIZE, int _numQuads = D_NUM_QUADS, int _height = D_HEIGHT);
// 	~Generator();
//
// 	bool generateChunk(long _x, long _y);
// 	bool generateChunkFromPos(float _x, float _y);
//
// 	Transform gTrans(long _x, long _y)
// 	{
// 		return m_chunkArray[_x][_y]->getTransform();
// 	}
// 	Mesh* gMesh(long _x, long _y)
// 	{
// 		return m_chunkArray[_x][_y]->m_mesh;
// 	}
// 	
// 	//float getTerHeightFromPos(float _x, float _z);
// private:
// 	float m_chunkSize;
// 	int m_numQuads;
// 	int m_height;
//
// 	//later made by list, generator will put there new chunks and check them to remove old
// 	//draw chunks by passing world coord of camera, which will choose which chunks to draw
// 	Chunk*** m_chunkArray;
//
// 	void initChunkArray();
//
//
// };
//
//

#pragma once

#include "../main.hpp"

#include "../rendering/Mesh.hpp"

#include <memory>
#include <mutex>

#ifdef PERLIN
class PerlinNoise;
#else 
class ValueNoise;
#endif // end of PERLIN


// class Transform;

#include "../core/Transform.hpp"

//TODO: 2 konstruktory, jeden normal, jeden "Copy" ktery ale dela dite
//TODO: co jde vzit od rodice referenci
//TODO: co jde nastavit podle rodice (/2 apod.)
//TODO: copy constructor by mohl byt i private

enum Corner : unsigned int {
	TL = 0, // Top Left child
	TR = 1, // Top Right child
	BL = 2, // Bottom Left child
	BR = 3, // Bottom Right child
};

typedef struct QuadTreeData {
#ifdef PERLIN
	QuadTreeData( float _offX, float _offZ, unsigned int _res,   unsigned int _maxLOD, PerlinNoise* _pn ) :
#else 
	QuadTreeData( float _offX, float _offZ, unsigned int _res,   unsigned int _maxLOD, ValueNoise* _pn ) :
#endif // end of PERLIN
		offsetX(_offX),
		offsetZ(_offZ),
		resolution(_res),
		maxLOD(_maxLOD),
		//curentLOD(0),
		generator(_pn)
	{
		transform = std::make_unique<Transform>( glm::vec3 (offsetX, 0.f, offsetZ));
	}

	float offsetX; // offset of the quadtree from world
	float offsetZ;

	unsigned int resolution; // resolution of terrain terrainCell = m_resolution x m_resolution quads
	unsigned int maxLOD;    // max LOD level for the whole quadtree
	//unsigned int curentLOD; // current higher LOD level that is present

#ifdef PERLIN
	PerlinNoise* generator; // noise generator
#else 
	ValueNoise* generator; // noise generator
#endif // end of PERLIN


	std::unique_ptr<Transform> transform;    // transform of the quadtree
} QuadTreeData;

typedef struct CellData {
	CellData() :
		isLeaf(true),
		LOD(1),
		rootOffX(0.f),
		rootOffZ(0.f),
		rootTexOffX(0.f),
		rootTexOffZ(0.f)
	{
	}

	void createRootData(float _size, unsigned int _resolution ) 
	{
		size = _size;
		quadSize = size / (float) (_resolution - 1);
		texInc = 1.f / (float) (_resolution - 1);
		texOffset = 1.f;
	}
	void init(const CellData& _parent, Corner _corner) 
	{

		size = _parent.size * 0.5f;
		quadSize = _parent.quadSize * 0.5f;
		LOD = _parent.LOD + 1;

		//the texCoord diference between two neighbourgh vertexes
		texInc = _parent.texInc * 0.5f;
		texOffset = _parent.texOffset * 0.5f;
		debug("lod " + std::to_string(LOD) + " texOffset " + std::to_string(texOffset) + " \ntexInc " + std::to_string(texInc));

		switch (_corner) {
			case(TL) :	rootOffX = _parent.rootOffX;
						rootOffZ = _parent.rootOffZ;
						rootTexOffX = _parent.rootTexOffX;
						rootTexOffZ = _parent.rootTexOffZ;
						break;;

			case(TR) :	rootOffX = _parent.rootOffX;
						rootOffZ = _parent.rootOffZ + size;
						rootTexOffX = _parent.rootTexOffX;
						rootTexOffZ = _parent.rootTexOffZ + texOffset;
					 	break;;

			case(BL) :	rootOffX = _parent.rootOffX + size;
						rootOffZ = _parent.rootOffZ;
						rootTexOffX = _parent.rootTexOffX + texOffset;
						rootTexOffZ = _parent.rootTexOffZ;
						break;;

			case(BR) :	rootOffX = _parent.rootOffX + size;
						rootOffZ = _parent.rootOffZ + size;
						rootTexOffX = _parent.rootTexOffX + texOffset;
						rootTexOffZ = _parent.rootTexOffZ + texOffset;
						break;;

			default :	error("wrong quadtree corner");
		}
		debug("parentoff " + std::to_string(rootTexOffX) + " " + std::to_string(rootTexOffZ));
	}
	// these variables are changing for every LOD level (child level)
	bool isLeaf;
	float size;           // size of this LOD level cell side
	float quadSize;		// size of one quad in this LOD level cell
	unsigned int LOD;     // LOD level of this cell

	float rootOffX;
	float rootOffZ;

	float rootTexOffX;
	float rootTexOffZ;
	float texInc;
	float texOffset;
	std::mutex m_mutex;

} CellData;

class TerrainCell {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
#ifdef PERLIN
	TerrainCell( float _offX, float _offZ, float _size, unsigned int _res , unsigned int _maxLOD, PerlinNoise* _pn );
#else 
	TerrainCell( float _offX, float _offZ, float _size, unsigned int _res , unsigned int _maxLOD, ValueNoise* _pn );
#endif // end of PERLIN


	~TerrainCell();

	 void addLODlevel();
	void delLODlevel();
	void updateTree( const glm::vec3& _camPos);
	
	void render() ;

	//TMP
	bool dataRdy;
	bool renderingRdy;

	glm::vec3 getAbsolutePos()
	{
		return m_qtData->transform->getAbsolutePos();

	}

	const glm::mat4& getTransMat()
	{
		return m_qtData->transform->getTransMat();
	}

private:
	// PRIVATE MEMBERS
	QuadTreeData* m_qtData;
	CellData	m_cellData;
	
	IndexedModel m_model; // model struct for this LOD level, holding verticies etc..
	Mesh* m_mesh;		 // mesh of this LOD level, holding pointers to GPU memory
	
	// Quadtree stuff
	TerrainCell* m_parent;    
	std::unique_ptr<TerrainCell> m_children[4];

	// PRIVATE METHODS
	TerrainCell(TerrainCell* _parent, Corner _corner); //Constructor used to create children from root node

	void updateNode(float _dist);
	void updateNode( const glm::vec3& _camPos);
	void renderLeaf() ;
	bool prepareRootMesh();
	bool prepareLeafMesh(Corner _corner);
	bool createMesh();
};



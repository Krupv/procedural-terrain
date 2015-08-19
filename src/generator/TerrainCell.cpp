#include "TerrainCell.hpp"

#ifdef PERLIN
#include "noises/PerlinNoise.hpp"
#else 
#include "noises/ValueNoise.hpp"
#endif // end of PERLIN


// #include "../core/Transform.hpp"




// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR FOR ROOT NODE
////////////////////////////////////////////////////////////////////////////////
#ifdef PERLIN
TerrainCell::TerrainCell ( float _offX, float _offZ, float _size, unsigned int _res,   unsigned int _maxLOD, PerlinNoise* _pn )
#else 
TerrainCell::TerrainCell ( float _offX, float _offZ, float _size, unsigned int _res,   unsigned int _maxLOD, ValueNoise* _pn )
#endif // end of PERLIN
{
	dataRdy = false;
	renderingRdy = false;

	m_qtData = new QuadTreeData(_offX, _offZ, _res,  _maxLOD, _pn);

	m_parent = nullptr;
	m_cellData.createRootData(_size, _res);

	dataRdy = prepareRootMesh();
		
}

TerrainCell::TerrainCell(TerrainCell* _parent, Corner _corner ) :
	m_parent(_parent),
	m_qtData(_parent->m_qtData),
	dataRdy(false),
	renderingRdy(false)
{
	// dataRdy = false;
	// renderingRdy = false;
    //
	// m_parent = _parent;
	// m_qtData = m_parent->m_qtData;
	m_cellData.isLeaf = true;
	m_cellData.init(m_parent->m_cellData, _corner);
	dataRdy = prepareLeafMesh(_corner);

}
////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
TerrainCell::~TerrainCell ()
{
	//if (m_cellData.LOD == 0)
	//{
	//	delete m_qtData;
	//}
}


void TerrainCell::addLODlevel()
{
	 if (m_cellData.LOD == m_qtData->maxLOD )
	 	return;

	std::lock_guard<std::mutex> lock(m_cellData.m_mutex);
	for ( unsigned int  i = 0; i < 4; i++ )
	{
		TerrainCell* child = new TerrainCell(this, (Corner) i); // to acces private constructor
		m_children[i] = std::unique_ptr<TerrainCell>(child);
	}
	m_cellData.isLeaf = false;	
}

void TerrainCell::delLODlevel()
{
	m_cellData.isLeaf = true;
	std::lock_guard<std::mutex> lock(m_cellData.m_mutex);
	for ( unsigned int  i = 0; i < 4; i++ )
	{
		m_children[i].reset();
	}
}

// PRIVATE
//------------------------------------------------------------------------------
//
bool TerrainCell::prepareRootMesh()
{
	//tmp bachelor
	stimer("noise generation");	

	unsigned int vecSize =  (m_qtData->resolution + 1) * (m_qtData->resolution + 1); // verticies in quadarray
	unsigned int indicSize =  m_qtData->resolution * m_qtData->resolution * 6; // 6 indices in one quad
	m_model.positions.reserve(vecSize);
	m_model.texCoords.reserve(vecSize);
	m_model.normals.reserve(vecSize);
	m_model.indices.reserve(indicSize);

	for ( unsigned int ix = 0; ix < m_qtData->resolution; ix++ )
	{
		for ( unsigned int  iz = 0; iz < m_qtData->resolution; iz++ )
		{
			float modelX = (ix * m_cellData.quadSize) + m_cellData.rootOffX;
			float modelZ = (iz * m_cellData.quadSize) + m_cellData.rootOffZ;
			 float modelY = m_qtData->generator->createNoise2d(modelX + m_qtData->offsetX, modelZ + m_qtData->offsetZ);
			//float modelY = 0.f;
			m_model.positions.push_back( glm::vec3 ( modelX, modelY, modelZ ));
			m_model.texCoords.push_back( glm::vec2 ( (float) ix * m_cellData.texInc, (float) iz * m_cellData.texInc ));
			m_model.normals.push_back( glm::vec3 ( 0.f, 0.f, 0.f ));
			
		}
	}
	
	for ( unsigned int ix = 0; ix < m_qtData->resolution - 1; ix++ )
	{
		for ( unsigned int  iz = 0; iz < m_qtData->resolution - 1; iz++ )
		{
			unsigned int vertIndex = (ix * m_qtData->resolution) + iz;
			m_model.indices.push_back(vertIndex);
			m_model.indices.push_back(vertIndex + m_qtData->resolution + 1);
			m_model.indices.push_back( vertIndex + 1);

			//model.botto t.push_back(
			m_model.indices.push_back( vertIndex);
			m_model.indices.push_back( vertIndex + m_qtData->resolution );
			m_model.indices.push_back( vertIndex + m_qtData->resolution + 1);
		}
	}
	etimer("noise generation");
	m_model.calcNormals();

	return true;
}

bool TerrainCell::prepareLeafMesh(Corner _corner)
{
	unsigned int vecSize =  (m_qtData->resolution + 1) * (m_qtData->resolution + 1); // verticies in quadarray
	unsigned int indicSize =  m_qtData->resolution * m_qtData->resolution * 6; // 6 indices in one quad

	// copy edges from parent
	
	m_model.positions.reserve(vecSize);
	m_model.texCoords.reserve(vecSize);
	m_model.normals.reserve(vecSize);
	m_model.indices.reserve(indicSize);

	// offset from parent based on which corner this child node ocupies
	// used to acces the correct parent index when copying model data
	unsigned int  parentOffX;
	unsigned int  parentOffZ;
	switch (_corner) {
		case(TL) :	parentOffX = 0;
					parentOffZ = 0;
					break;;

		case(TR) :	parentOffX = 0 ;
					parentOffZ = m_qtData->resolution;
					break;;

		case(BL) :	parentOffX = m_qtData->resolution ;
					parentOffZ = 0;
					break;;

		case(BR) :	parentOffX = m_qtData->resolution ;
					parentOffZ = m_qtData->resolution ;
					break;;

		default :	error("wrong quadtree corner");
	}





	for ( unsigned int ix = 0; ix < m_qtData->resolution; ix++ )
	{
		for ( unsigned int  iz = 0; iz < m_qtData->resolution; iz++ )
		{

			if ( ((ix % 2) == 0) && ((iz % 2) == 0))
			{
				//copy data from parent model on halfX halfZ index (since child has 2 times resolution of parent)
				unsigned int pix = (ix + parentOffX) / 2 ;
				unsigned int piz = (iz + parentOffZ) / 2;
				unsigned int index = (pix * m_qtData->resolution) + piz; // index in parent vector

				m_model.positions.push_back( m_parent->m_model.positions[ index ]);
				m_model.texCoords.push_back( m_parent->m_model.texCoords[ index ]);
				// m_model.normals.push_back( m_parent->m_model.normals[ index ]);
				m_model.normals.push_back( glm::vec3 ( 0.f, 0.f, 0.f ));

				// m_model.texCoords.push_back( glm::vec2 ( (float) ix * texInc, (float) iz * texInc ));
				// m_model.texCoords.push_back( glm::vec2 ( ((float) ix * m_cellData.texInc) + m_cellData.rootTexOffX, ((float) iz * m_cellData.texInc) + m_cellData.rootTexOffZ ));


			}
			else
			{
				//generate higher res data
				float modelX = (ix * m_cellData.quadSize) + m_cellData.rootOffX;
				float modelZ = (iz * m_cellData.quadSize) + m_cellData.rootOffZ;
				float modelY = m_qtData->generator->createNoise2d(modelX + m_qtData->offsetX, modelZ + m_qtData->offsetZ);

				m_model.positions.push_back( glm::vec3 ( modelX, modelY, modelZ ));

				// m_model.texCoords.push_back( glm::vec2 ( (float) ix * m_cellData.texInc, (float) iz * texInc ));
				m_model.texCoords.push_back( glm::vec2 ( ((float) ix * m_cellData.texInc) + m_cellData.rootTexOffX, ((float) iz * m_cellData.texInc) + m_cellData.rootTexOffZ ));

				m_model.normals.push_back( glm::vec3 ( 0.f, 0.f, 0.f ));
			}

			
		}
	}
	
	//STICHING
	unsigned int index = (m_qtData->resolution ) * (m_qtData->resolution - 1);
	for ( unsigned int i = 1; i < (m_qtData->resolution - 1); i+=2 )
	{
		float y0 = m_model.positions[i-1].y;
		float y2 = m_model.positions[i+1].y;
		m_model.positions[i].y = (y0 + y2) * 0.5f;

		y0 = m_model.positions[index + i-1].y;
		y2 = m_model.positions[index + i+1].y;
		m_model.positions[index + i].y = (y0 + y2) * 0.5f;
	}

	for ( unsigned int i = 1; i < (m_qtData->resolution - 1); i+=2 )
	{
		float y0 = m_model.positions[ (i-1) * m_qtData->resolution ].y;
		float y2 = m_model.positions[ (i+1) * m_qtData->resolution ].y;
		m_model.positions[ i * m_qtData->resolution ].y = (y0 + y2) * 0.5f;

		y0 = m_model.positions[ (i-1) * m_qtData->resolution + m_qtData->resolution- 1 ].y;
		y2 = m_model.positions[ (i+1) * m_qtData->resolution + m_qtData->resolution- 1 ].y;
		m_model.positions[ i * m_qtData->resolution + m_qtData->resolution- 1 ].y = (y0 + y2) * 0.5f;
	}
	

	for ( unsigned int ix = 0; ix < m_qtData->resolution - 1; ix++ )
	{
		for ( unsigned int  iz = 0; iz < m_qtData->resolution - 1; iz++ )
		{
			unsigned int vertIndex = (ix * m_qtData->resolution) + iz;
			m_model.indices.push_back(vertIndex);
			m_model.indices.push_back(vertIndex + m_qtData->resolution + 1);
			m_model.indices.push_back( vertIndex + 1);

			//model.botto t.push_back(
			m_model.indices.push_back( vertIndex);
			m_model.indices.push_back( vertIndex + m_qtData->resolution );
			m_model.indices.push_back( vertIndex + m_qtData->resolution + 1);
		}
	}
m_model.calcNormals();

	return true;
}

void TerrainCell::updateTree( const glm::vec3& _camPos)
{
	// glm::vec3 tpos = m_qtData->transform->getAbsolutePos();
	// tpos.x += m_cellData.size * 0.5f;
	// tpos.z += m_cellData.size * 0.5f;
	// //tpos.y += m_cellData.size * 0.5f;
	// float dist = glm::distance(  glm::vec2 (_camPos.x, _camPos.z), glm::vec2 (tpos.x, tpos.z) );
	// this->updateNode(dist);
	 this->updateNode(_camPos);
}

void TerrainCell::updateNode( const glm::vec3& _camPos)
{
	glm::vec3 tpos = m_qtData->transform->getAbsolutePos();
	tpos.x += (m_cellData.size  * 0.5f)+ m_cellData.rootOffX;
	tpos.z += (m_cellData.size * 0.5f)+ m_cellData.rootOffZ;
	//tpos.y += m_cellData.size * 0.5f;
	//float _dist = glm::distance(  glm::vec2 (_camPos.x, _camPos.z), glm::vec2 (tpos.x, tpos.z) );
	float _dist = glm::distance(_camPos, tpos);


//#define P 4.f
	//const float P = m_qtData->maxLOD ;
	const float P = 1.f ;
	//const float P = 0.5f ;

	if (!m_cellData.isLeaf)
	{
		if (_dist > (m_cellData.size * P )  ) 
			delLODlevel();
		else if (_dist < (m_cellData.size * P ) )
		{
			 for ( int i = 0; i < 4; i++ )
			 {
			;
				m_children[i]->updateNode(_camPos);			
				//m_children[i]->addLODlevel();			
			 }
		 }

	}
	else if (_dist < (m_cellData.size * P ))
		addLODlevel();

}

// void TerrainCell::updateNode( float _dist)
// {
// 	float halfSize = m_cellData.size * 0.5f;
//
// 	if (!m_cellData.isLeaf)
// 	{
// 		if (_dist > (m_cellData.size * 1.6f )  ) 
// 			delLODlevel();
// 		else if (_dist < (m_cellData.size * 1.5f) )
// 		{
// 			 for ( int i = 0; i < 4; i++ )
// 			 {
// 			;
// 				m_children[i]->updateNode(_dist);			
// 				//m_children[i]->addLODlevel();			
// 			 }
// 		 }
//
// 	}
// 	else if (_dist < (m_cellData.size * 1.5f))
// 		addLODlevel();
// 		
//
//
// }

bool TerrainCell::createMesh()
{
	if (dataRdy)
	{
		m_mesh = new Mesh(m_model);
		renderingRdy = true;
	}
	
	return renderingRdy;
}


void TerrainCell::renderLeaf() 
{
	if (m_cellData.LOD > 1)
	{
		// debug("rendering " + std::to_string(m_cellData.rootOffX) + std::to_string(m_cellData.rootOffZ));
		// for ( int i = 0; i < m_model.positions.size(); i++ )
		// {
		// 	debug("vert " + std::to_string(i) + " " + std::to_string(m_model.positions[i].x) + " " + std::to_string(m_model.positions[i].x));
		// 	
		// }
		
	}
	if (!renderingRdy)
	{
		if ( ! createMesh() )
		{
			return;
		}
		//createMesh();
	}
	m_mesh->draw();

}

void TerrainCell::render() 
{
	std::lock_guard<std::mutex> lock(m_cellData.m_mutex);

	if(m_cellData.isLeaf)
		renderLeaf();
	else
	{
		 for ( int i = 0; i < 4; i++ )
		 {
		 	m_children[i]->render();
		 }
	}

}
















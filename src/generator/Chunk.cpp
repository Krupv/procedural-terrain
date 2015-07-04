//#include "Chunk.hpp"
//
//#include <cmath>
//#include <iostream>
//
//
//////////////////////////////////////////////////////////////////////////////////
//// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////
//
//#ifdef PERLIN
//Chunk::Chunk (float _offsetX, float _offsetY, int _nQuads, float _size, PerlinNoise* _noiseGen) : 
//#else
//Chunk::Chunk (float _offsetX, float _offsetY, int _nQuads, float _size, ValueNoise* _noiseGen) : 
//#endif
//	m_offX(_offsetX), 
//	m_offY(_offsetY),
//	m_numQuads(_nQuads),
//	// m_height(_height),
//	m_size(_size),
//	m_noise(_noiseGen)
//{
//	m_strip = STRIP_RES;
//	m_height = m_noise->getHeight();
//	renderingRdy = false;
//	dataRdy = false;
//	m_mesh = nullptr;
//	m_indicies = nullptr;
//	m_verticies = nullptr;
//	float posX = m_offX; // *  m_size;
//	float posZ = m_offY; // *  m_size;
//	m_transform = new Transform(glm::vec3(posX, 0.0, posZ));
//	m_quadSize = calculateQuadSize();
//	prepareMesh();
//
//	// genHeightMap();
//
//	dataRdy = true;
//	//createMesh();
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// DESTRUCTOR
//////////////////////////////////////////////////////////////////////////////////
//Chunk::~Chunk()
//{
//	if (m_mesh)
//		delete m_mesh;
//	if (m_indicies)
//		delete m_indicies;
//	if (m_verticies)
//		std::free( m_verticies);
//	if (m_transform)
//		delete m_transform;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// GENERATE HEIGHT MAP
//////////////////////////////////////////////////////////////////////////////////
//void Chunk::genHeightMap()
//{
//	unsigned int size = ((m_numQuads +  (m_strip*2)) * (m_numQuads +  (m_strip*2)));
//	for ( int i = 0; i < size; i++)
//	{
//		glm::vec3 pos = m_model.positions[i];
//		
//		pos.x += (float) ((m_offX  )); // * m_size;
//		pos.z += (float) ((m_offY )); // * m_size;
//		
//		//#ifdef PERLIN 
//		m_model.positions[i].y =   m_noise->createNoise2d(pos.x, pos.z);
//	//	#else 
//	//		m_verticies[i].getPos()->y = scaled_octave_noise_2d(15.0f, 0.42f, 0.00017f, 0, m_height , absf(pos.x), absf(pos.z));
//	//	#endif // end of MYNOISE 
//	}
//	m_model.calcNormals();
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// CALCULATE QUAD SIZE
//////////////////////////////////////////////////////////////////////////////////
//float Chunk::calculateQuadSize() const
//{
//	return m_size / (float) (m_numQuads -1 );
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// CREATE MESH
//////////////////////////////////////////////////////////////////////////////////
//void Chunk::createMesh()
//{
//	// m_mesh = new Mesh(m_verticies,(unsigned int) ( (m_numQuads + 1) * (m_numQuads +1)), m_indicies, (unsigned int) (m_numQuads * m_numQuads * 3 * 2), true ); 
//	m_mesh = new Mesh(m_model); 
//
//	renderingRdy = true;
//	//TODO: create array for heights before deleting veriticies
//	// std::free(m_verticies);
//	// delete m_indicies;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// PREPARE MESH DATA
//////////////////////////////////////////////////////////////////////////////////
//void Chunk::prepareMesh() 
//{
//	m_model.positions.reserve( (m_numQuads + m_strip ) * (m_numQuads + m_strip ) );
//	m_model.texCoords.reserve( (m_numQuads + 1 ) * (m_numQuads + 1 ) );
//	m_model.normals.reserve( (m_numQuads + 1 ) * (m_numQuads + 1 ) );
//
//	m_model.indices.reserve( m_numQuads * m_numQuads * 6); // 6 = number of indicies in one quad (2 triangles, each 3 verticies(nonindexed))
//
//	float texInc = 1.f / (float) (m_numQuads -1);
//
//	int index = 0;
//	for (int i = 0 - m_strip; i < m_numQuads + m_strip ; i++)
//	{
//		for (int j = 0 - m_strip; j < m_numQuads + m_strip ; j++)
//		{
//			float modelX = j * m_quadSize;
//			float modelZ = i * m_quadSize;
//			float modelY = m_noise->perlinNoise2d(modelX + m_offX, modelZ + m_offY);
//			m_model.positions.push_back(glm::vec3( modelX, modelY, modelZ));
//			m_model.texCoords.push_back (glm::vec2((float)j * texInc, (float) i * texInc));
//			m_model.normals.push_back(glm::vec3(0,0,0));
//		}
//	}
//
//std::cout << "asd " << m_model.positions.size() << std::endl;
//	index = 0;
//
//	for ( int j = m_strip; j < m_numQuads +m_strip -1 ; j++ )
//	{
//		for ( int k = m_strip; k < m_numQuads +m_strip -1 ;k++ )
//		{
//			int vertIndex = (j * (m_numQuads + m_strip *2 ) ) + k;
//			//top tri
//			m_model.indices.push_back(vertIndex);
//			m_model.indices.push_back(vertIndex + (m_numQuads+m_strip*2) + 1);
//			m_model.indices.push_back( vertIndex + 1);
//
//			//model.botto t.push_back(
//			m_model.indices.push_back( vertIndex);
//			m_model.indices.push_back( vertIndex + (m_numQuads+m_strip*2) );
//			m_model.indices.push_back( vertIndex + (m_numQuads+m_strip*2) + 1);
//		}
//	}
//	m_model.calcNormals();
//}
//
//
//
//
//
//
//
//
//
////float linearInterpolate(float a, float b, float x)
////{
////	if (x < 0.f)
////		x = 0.f;
////	else if (x > 1.f)
////		x = 1.f;
////	return a * (1-x) + b*x;
////}
//
////float Chunk::getHeightFromRelPos(float _x, float _z)
////{
////	//TODO: choose verticies based on the closest one to the position
////	if ((_x > m_size) || (_z > m_size))
////	{
////		std::cerr << "out of bound coordinations, use getHeightFromAbsPos" << std::endl;
////		std::cerr << _x << " " << _z  << std::endl;
////		
////		
////	}
////
////
////	int quadX = (int) (_x / m_quadSize);
////	int quadZ = (int) (_z / m_quadSize);
////
////	float relX = fmod(_x, m_quadSize);
////	float relZ = fmod(_z, m_quadSize);
////
////	float QuadCenter = m_quadSize / 2.f;
////
////	int quadIndex;
////
////	float bottomRight = m_verticies[ (quadZ * m_numQuads) + quadX ].getPos()->y;
////	float bottomLeft = m_verticies[ (quadZ * m_numQuads) + quadX + 1 ].getPos()->y;
////	float topRight = m_verticies[ (quadZ * m_numQuads) + m_numQuads + quadX ].getPos()->y;
////	float topLeft = m_verticies[ (quadZ * m_numQuads) + m_numQuads + quadX +1 ].getPos()->y;
////
////	float lineAweight = ((relX / m_quadSize) * 0.7 ) + ((relZ / m_quadSize) * 0.7f);
////
////	float wx,wz;
////	if (relX > relZ)
////	{
////		wx = 0.7f;
////		wz = 0.3f;
////	}
////	else if (relX < relZ)
////	{
////		wx = 0.3f;
////		wz = 0.7f;
////	}
////	else 
////	{
////		wx = 0.5f;
////		wz = 0.5f;
////	}
////	float lineBweight = ((relX / m_quadSize) * wx) + ((relZ / m_quadSize) * wz);
////	float lineA = linearInterpolate(bottomRight, topLeft, lineAweight);
////	float lineB = linearInterpolate(bottomLeft, topRight, lineBweight);
////	return linearInterpolate(lineA, lineB, 0.5f);
////}
//
//

// #include "Generator.hpp"
//
// #include <glm/glm.hpp>
// //#include <cstdlib>
// #include <math.h> //fmod
//
// ////////////////////////////////////////////////////////////////////////////////
// // CONSTRUCTOR
// ////////////////////////////////////////////////////////////////////////////////
// Generator::Generator(float _size, int _numQuads, int _height) : 
// 	m_chunkSize(_size),
// 	m_numQuads(_numQuads),
// 	m_height(_height)
// {
// 	counter = 0;
// 	for ( int i = 0; i < TMP ; i++ )
// 		m_chList[i] = nullptr;
// 	
// 	initChunkArray();	
// }
//
//
// ////////////////////////////////////////////////////////////////////////////////
// // DESTRUCTOR
// ////////////////////////////////////////////////////////////////////////////////
// Generator::~Generator()
// {
//
// }
//
//
// ////////////////////////////////////////////////////////////////////////////////
// // INIT CHUNK ARRAY
// ////////////////////////////////////////////////////////////////////////////////
// void Generator::initChunkArray()
// {
// 	m_chunkArray = new Chunk**[MAX_ARRAY_SIZE];
//
// 	for ( long i = 0; i < MAX_ARRAY_SIZE ; i++ )
// 	{
// 		m_chunkArray[i] = new Chunk*[MAX_ARRAY_SIZE];		
// 		for ( long j = 0; j < MAX_ARRAY_SIZE ; j++ )
// 		{
// 			m_chunkArray[i][j]= nullptr;
// 		}
// 		
// 	}
// 	
// }
//
//
// ////////////////////////////////////////////////////////////////////////////////
// // GENERATE CHUNK
// ////////////////////////////////////////////////////////////////////////////////
// bool Generator::generateChunk(long _x, long _y)
// {
// 	if ( m_chunkArray[_x][_y] == nullptr)
// 	{
// 		m_chunkArray[_x][_y] = new Chunk(_x - (MAX_ARRAY_SIZE / 2) ,_y - (MAX_ARRAY_SIZE / 2), m_numQuads,m_height,m_chunkSize);
//
// 		m_chList[counter++] = m_chunkArray[_x][_y];
// 	}
// 	return ( m_chunkArray[_x][_y] != nullptr );
//
// }
//
//
// ////////////////////////////////////////////////////////////////////////////////
// // GENERATE CHUNK FROM POSITION
// ////////////////////////////////////////////////////////////////////////////////
// bool Generator::generateChunkFromPos(float _x, float _y)
// {
// 	long x,y;
// 	x = (long) ( (_x - m_chunkSize * 0.5f ) /  m_chunkSize) + (long) (MAX_ARRAY_SIZE / 2);
// 	y = (long) ( (_y - m_chunkSize * 0.5f) /  m_chunkSize) + (long) (MAX_ARRAY_SIZE / 2);
//
// 	
//
// 	generateChunk(x   ,y);
//
// 	generateChunk(x-1 ,y);
// 	generateChunk(x+1 ,y);
//
// 	generateChunk(x   ,y-1);
// 	generateChunk(x   ,y+1);
//
// 	generateChunk(x-1 ,y-1);
// 	generateChunk(x+1 ,y+1);
// 	generateChunk(x-1 ,y+1);
// 	generateChunk(x+1 ,y-1);
//
// 	return true;
// }

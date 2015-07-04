#pragma once

#include "../main.hpp"
#include "../utils/Lua.hpp"


#include <thread>
#include <mutex>



#include "../entity/IEntity.hpp"
#include "TerrainCell.hpp"


#include "../rendering/BasicTerrainShader.hpp"
#include "../rendering/PhongTerrainShader.hpp"

#ifdef PERLIN
#include "noises/PerlinNoise.hpp"
#else 
#include "noises/ValueNoise.hpp"
#endif // end of PERLIN




//TMP
#include <SFML/Window/Keyboard.hpp>


#define LAN_SIZE 3 //default size of 2-dimensional terraincell array

#define DEF_CELL_SIZE 500.f //default terrain cell size
#define DEF_CELL_HEIGHT 200.f //default terrain height
#define DEF_CELL_RES 128 //default cell resolution (128x128) 

#define DEF_LOD_LEVEL 1

class Skydome;

class Landscape : public IEntity {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
	Landscape( const std::string& _luaFile, IEntity* _observer = nullptr );
	~Landscape();

	void setMaterial(const Material& _material)
	{
		m_material = Material(_material);
	}

	void setShader(IShader* _shader)
	{
		m_shader = _shader;
	}

#ifdef PERLIN
	PerlinNoise* getNoiseGenerator() { return m_noise; };
#else 
	ValueNoise* getNoiseGenerator() { return m_noise; };
#endif // end of PERLIN



private:
	Skydome* m_skydome;
	// PRIVATE MEMBERS
	std::unique_ptr<TerrainCell> m_terrain[LAN_SIZE][LAN_SIZE];
	std::mutex* m_mutexes[LAN_SIZE][LAN_SIZE];

	IShader* m_shader;
	float m_sunPos;

	Material m_material;
	float m_cellSize;
	float m_cellHigh;
	int m_cellResolution;
	unsigned int m_LOD;
	const int m_cellCount = 3;

#ifdef PERLIN
	PerlinNoise* m_noise;
#else 
	ValueNoise* m_noise;
#endif // end of PERLIN



	std::thread updateThread;
	bool runningThreaded;
	// std::mutex* m_mutex;

	void moveX(float _distance, const glm::vec3& _pos);
	void moveZ(float _distance, const glm::vec3& _pos);

	// PRIVATE METHODS
	void moveTerrain(bool r, bool l, bool u, bool d);
	void render(const Tmatricies& _pipe) const;
	void updateThreaded();
	void step();
	void update(float _delta);
};


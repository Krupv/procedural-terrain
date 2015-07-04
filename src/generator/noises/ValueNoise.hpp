#pragma once
#include "../../main.hpp"

#include <cmath>
#include <string>


#define DEF_PERSIST 0.5f
#define DEF_OCTAVES 6 
#define DEF_SCALE 1.f
#define DEF_HEIGH 10.f

class ValueNoise {

public:
	ValueNoise();
	ValueNoise( const std::string& _luaFile);
	void init( unsigned int _octaves, float _persistence, float _scale, float _maxHeight) 
	{
		m_octaves = _octaves;
		m_persistence = _persistence;
		m_scale = _scale;
		m_maxHeight = _maxHeight;
		debug("perlin noise " + std::to_string(m_octaves) + " " + std::to_string(m_persistence) + " " + std::to_string(m_scale) + " " + std::to_string(m_maxHeight));
	}

	~ValueNoise() {};
	float createNoise2d(float x, float y);

	float getScale() { return m_scale; };
	float getHeight() { return m_maxHeight; };
	
private:
	float noise(int x, int y);
	float smoothNoise(float x, float y);
	float interpolatedNoise(float x, float y);

	float cosineInterpolation(float a, float b, float x);

	unsigned int m_octaves;
	float m_persistence;
	float m_scale;
	float m_maxHeight;

	bool m_power;

};

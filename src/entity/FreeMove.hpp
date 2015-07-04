#pragma once
#include "IEntComponent.hpp"

#ifdef PERLIN
#include "../generator/noises/PerlinNoise.hpp"
#else 
#include "../generator/noises/ValueNoise.hpp"
#endif // end of PERLIN


// class PerlinNoise;

class FreeMove : public IEntComponent {
public:
#ifdef PERLIN
	FreeMove(float _speed, PerlinNoise _noise = PerlinNoise() );
#else 
	FreeMove(float _speed, ValueNoise _noise = ValueNoise() );
#endif // end of PERLIN



	~FreeMove();

	void update(float _delta);

private:
	float m_speed;
	float m_baseSpeed;

#ifdef PERLIN
	PerlinNoise pn;
#else 
	ValueNoise pn;
#endif // end of PERLIN


};

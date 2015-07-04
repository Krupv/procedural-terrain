#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>
#include <string>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

#define DEF_PERSIST 0.5f
#define DEF_OCTAVES 6 
#define DEF_SCALE 1.f
#define DEF_HEIGH 10.f

class PerlinNoise {
	// The permutation vector
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();

	PerlinNoise( const std::string& _luaFile);

	void init( unsigned int _octaves, float _persistence, float _scale, float _maxHeight);

	// Generate a new permutation vector based on the value of seed
	//PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double perlinNoise2d(double x, double y, double z);
	double createNoise2d(double _x, double _y);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	unsigned int m_octaves;
	float m_persistence;
	float m_scale;
	float m_maxHeight;

	bool m_power;
};

#endif

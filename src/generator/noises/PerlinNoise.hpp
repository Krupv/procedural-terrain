#ifndef PERLINNOISE_H
#define PERLINNOISE_H

//#include <vector>
//#include <string>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

#define DEF_PERSIST 0.5f
#define DEF_OCTAVES 6 
#define DEF_SCALE 1.f
#define DEF_HEIGH 10.f


// Multi-octave Simplex noise
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float octave_noise_2d(const float octaves,
const float persistence,
const float scale,
const float x,
const float y);
float octave_noise_3d(const float octaves,
const float persistence,
const float scale,
const float x,
const float y,
const float z);
float octave_noise_4d(const float octaves,
const float persistence,
const float scale,
const float x,
const float y,
const float z,
const float w);


// Scaled Multi-octave Simplex noise
// The result will be between the two parameters passed.
float scaled_octave_noise_2d( const float octaves,
const float persistence,
const float scale,
const float loBound,
const float hiBound,
const float x,
const float y);
float scaled_octave_noise_3d( const float octaves,
const float persistence,
const float scale,
const float loBound,
const float hiBound,
const float x,
const float y,
const float z);
float scaled_octave_noise_4d( const float octaves,
const float persistence,
const float scale,
const float loBound,
const float hiBound,
const float x,
const float y,
const float z,
const float w);

// Scaled Raw Simplex noise
// The result will be between the two parameters passed.
float scaled_raw_noise_2d( const float loBound,
const float hiBound,
const float x,
const float y);
float scaled_raw_noise_3d( const float loBound,
const float hiBound,
const float x,
const float y,
const float z);
float scaled_raw_noise_4d( const float loBound,
const float hiBound,
const float x,
const float y,
const float z,
const float w);


// Raw Simplex noise - a single noise value.
float raw_noise_2d(const float x, const float y);
float raw_noise_3d(const float x, const float y, const float z);
float raw_noise_4d(const float x, const float y, const float, const float w);


int fastfloor(const float x);

float dot(const int* g, const float x, const float y);
float dot(const int* g, const float x, const float y, const float z);
float dot(const int* g, const float x, const float y, const float z, const float w);


// The gradients are the midpoints of the vertices of a cube.
static const int grad3[12][3] = {
{1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
{1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
{0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};


// The gradients are the midpoints of the vertices of a hypercube.
static const int grad4[32][4]= {
{0,1,1,1}, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1},
{0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
{1,0,1,1}, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
{-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
{1,1,0,1}, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
{-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
{1,1,1,0}, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
{-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
};


// Permutation table. The same list is repeated twice.
static const int perm[512] = {
151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


// A lookup table to traverse the simplex around a given point in 4D.
static const int simplex[64][4] = {
{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};


#include <string>


class PerlinNoise {
public:

	PerlinNoise();

	PerlinNoise( const std::string& _luaFile);

	void init( unsigned int _octaves, float _persistence, float _scale, float _maxHeight);

	// Generate a new permutation vector based on the value of seed
	//PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double createNoise2d(double _x, double _y);

private:

	unsigned int m_octaves;
	float m_persistence;
	float m_scale;
	float m_maxHeight;

	bool m_power;

};


#endif

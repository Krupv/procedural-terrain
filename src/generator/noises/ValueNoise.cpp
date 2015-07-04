#include "ValueNoise.hpp"
#include "../../utils/Lua.hpp"

#include <cstdlib>
// #include <iostream>
#include <cmath>


ValueNoise::ValueNoise()
{
	m_power = false;
	init(DEF_OCTAVES, DEF_PERSIST, DEF_SCALE, DEF_HEIGH);
}

ValueNoise::ValueNoise( const std::string& _luaFile)
{
	Lua lua(_luaFile);
	unsigned int octaves = lua.getInt("octaves", DEF_OCTAVES);
	float persistance = lua.getFloat("persistence", DEF_PERSIST);
	float scale = lua.getFloat("scale", DEF_SCALE);
	float maxHeight = lua.getFloat("cell.height", DEF_HEIGH);
	m_power = lua.getBool("power", false);
	debug("wd" + std::to_string(m_power));


	init(octaves, persistance, scale, maxHeight);

}

float ValueNoise::createNoise2d(float _x, float _y)
{

	float x = _x * m_scale;
	float y = _y * m_scale;
	float total = 0.f;
	float frequency = m_persistence;
	float amplitude = 1.f;
	int n = m_octaves - 1;

	for ( int i = 0; i < n ; i++ )
	{
		total += interpolatedNoise(x*frequency, y *frequency) * amplitude;

		frequency *= 2.f;
		amplitude *= m_persistence;
	}

	if (m_power)
		return pow(total * m_maxHeight, 2.0f);
	else
		return total * m_maxHeight;

}

float ValueNoise::noise(int x, int y)
{
	int n = x + y * 57;
	float a = (n<<13) ^ n;

	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float ValueNoise::smoothNoise(float x, float y)
{
	float corners = ( noise(x-1, y-1) + noise(x+1,y-1) + noise(x-1,y+1) + noise(x+1,y+1)) / 16.f;
	float sides = ( noise(x-1, y) + noise(x+1,y) + noise(x,y-1) + noise(x,y+1)) / 8.f;
	float center = noise(x, y) /4.f;
	float res = corners + sides + center;
	return res;
	
}

float ValueNoise::interpolatedNoise(float x, float y)
{
	int integer_X = floor(x);
	float fract_X = x - integer_X;

	int integer_Y = floor(y);
	float fract_Y = y - integer_Y;

	float v1 = smoothNoise(integer_X, integer_Y);
	float v2 = smoothNoise(integer_X + 1, integer_Y);
	float v3 = smoothNoise(integer_X, integer_Y + 1);
	float v4 = smoothNoise(integer_X + 1, integer_Y + 1);

	float i1 = cosineInterpolation(v1, v2, fract_X);
	float i2 = cosineInterpolation(v3, v4, fract_X);

	return cosineInterpolation(i1, i2, fract_Y);
}

float ValueNoise::cosineInterpolation(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1- cosf(ft)) * 0.5f;
	return a*(1-f) + b*f;
	// return  a*(1-x) + b*x;
}

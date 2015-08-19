#include "FreeMove.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <iostream>


// #include "../generator/noises/MyPerlin.hpp"

#define SPRINT_SPEED 50.f
#define GRAVITY -60.f

// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
#ifdef PERLIN
FreeMove::FreeMove(float _speed, PerlinNoise _noise ) : m_speed(_speed)
#else 
FreeMove::FreeMove(float _speed, ValueNoise _noise ) : m_speed(_speed)
#endif // end of PERLIN
{
	m_baseSpeed = m_speed;

	pn = _noise;
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
FreeMove::~FreeMove ()
{

}


////////////////////////////////////////////////////////////////////////////////
// UPDATE
////////////////////////////////////////////////////////////////////////////////
void FreeMove::update(float _delta)
{

	glm::vec3 pos = m_owner->getAbsolutePos();
	//float terrainY = scaled_octave_noise_2d(10.f, 0.42f, 0.00017f, 0, 2000, pos.x, pos.z);
	float terrainY = pn.createNoise2d(pos.x, pos.z);
	//float terrainY = -20.f;

	if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			&& (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) )
	{
		m_owner->moveForwardCollisY(m_speed/2.f * _delta, terrainY);
		m_owner->moveRightCollisY(-m_speed/2.f * _delta, terrainY);
	}
	else if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				&& (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) )
	{
		m_owner->moveForwardCollisY(-m_speed/2.f * _delta, terrainY);
		m_owner->moveRightCollisY(m_speed/2.f * _delta, terrainY);
	}
	else if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			&& (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) )
	{
		m_owner->moveForwardCollisY(m_speed/2.f * _delta, terrainY);
		m_owner->moveRightCollisY(-m_speed/2.f * _delta, terrainY);
	}
	else if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			&& ( sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) )
	{
		m_owner->moveForwardCollisY(m_speed/2.f * _delta, terrainY);
		m_owner->moveRightCollisY(m_speed/2.f * _delta, terrainY);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
		m_owner->moveForwardCollisY(-m_speed * _delta, terrainY);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
		m_owner->moveForwardCollisY(m_speed * _delta, terrainY);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
		m_owner->moveRightCollisY(-m_speed * _delta, terrainY);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
		m_owner->moveRightCollisY(m_speed * _delta, terrainY);

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		m_speed = m_baseSpeed;


		//gravity
		//m_owner->moveCollisY( glm::vec3(0,-1,0), GRAVITY * _delta, terrainY + 2.f);
	}
	else
		m_speed = SPRINT_SPEED * m_baseSpeed;

//	std::cout << "cam x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;

}

// PRIVATE
//------------------------------------------------------------------------------



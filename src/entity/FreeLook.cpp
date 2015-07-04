#include "FreeLook.hpp"

#include <SFML/Window/Mouse.hpp>


// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
FreeLook::FreeLook (float _speed, sf::Vector2i _center) : 
	m_speed(_speed), 
	m_center(_center)
{
	m_paused = false;
};
	

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
FreeLook::~FreeLook ()
{

}


////////////////////////////////////////////////////////////////////////////////
// UPDATE
////////////////////////////////////////////////////////////////////////////////
void FreeLook::update(float _delta)
{
	if (!m_paused)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();
		sf::Vector2i delta = mousePos - m_center;
		sf::Mouse::setPosition(m_center);


		if (delta.x != 0)
			m_owner->rotate(glm::normalize(glm::vec3(0,1,0)), (float) delta.x * -m_speed  );
		if (delta.y != 0)
			m_owner->rotate(m_owner->getRight(), (float) delta.y *  -m_speed );

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			m_paused = true;

	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			m_paused = false;

	}
}


// PRIVATE
//------------------------------------------------------------------------------





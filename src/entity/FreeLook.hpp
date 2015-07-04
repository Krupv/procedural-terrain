#pragma once

#include "IEntComponent.hpp"
#include <SFML/System/Vector2.hpp>


class FreeLook : public IEntComponent {
public:
	FreeLook(float _speed, sf::Vector2i _center);
	~FreeLook();

	void update(float _delta);

private:
	float m_speed;
	sf::Vector2i m_center;
	
	bool m_paused;
};

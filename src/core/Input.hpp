// #pragma once
// #include "../rendering/Display.hpp"
//
//
// class Input {
// public:
// 	Input(DDisplay* _window) 
// 	{
// 		m_window = _window;	
// 		setMouseCenter();
// 	};
//
// 	void update()
// 	{
// 		m_mousePos = sf::Mouse::getPosition();
// 	}
// 	sf::Vector2i getMouseDelta()
// 	{
// 		sf::Vector2i old = m_mousePos;
// 		m_mousePos = m_mouseCenter;
// 		sf::Mouse::setPosition(m_mouseCenter);
// 		return  old - m_mousePos;
// 	}
//
// 	void setMouseCenter() {
// 		m_mouseCenter.x =  m_window->getWidth() / 2;
// 		m_mouseCenter.y =  m_window->getHeight() / 2;
// 		sf::Mouse::setPosition(m_mouseCenter);
// 		
// 	}
// private:
// 	sf::Vector2i m_mousePos;
// 	sf::Vector2i m_mouseCenter;
//
// 	DDisplay* m_window;
//
// };

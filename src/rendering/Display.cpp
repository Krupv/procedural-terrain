#include "Display.hpp"

#include <GL/glew.h>

#ifdef None
	#undef None // SFML/Window is using None
#endif 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <stdlib.h> // EXIT_SUCCESS
					// EXIT_FAILURE

// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
DDisplay::DDisplay( int _width, int _height, const std::string& _title )
{

	sf::ContextSettings openglSettings;

	openglSettings.depthBits = 24;
	openglSettings.stencilBits = 0;
	openglSettings.antialiasingLevel = 0;
	openglSettings.majorVersion = 2;
	openglSettings.minorVersion = 1;

	m_running = true;
	m_paused = false;

	m_window = new sf::Window(sf::VideoMode(_width, _height), _title, sf::Style::Default, openglSettings);

	if ( !m_window )
	{
		error("Error: Could not create glfw window");
		exit(EXIT_FAILURE);
	}
	m_window->setPosition(sf::Vector2i(0, 0));
	m_window->setMouseCursorVisible(false);

	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		error("Error: Could not initialize GLEW");
		m_window->close();
		exit(EXIT_FAILURE);
	}

	//m_window->setFramerateLimit(0);
	m_window->setFramerateLimit(60);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);

	clear(0,0,0,0);
	update();
	
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
DDisplay::~DDisplay()
{
	m_window->close();
	delete m_window;
}


////////////////////////////////////////////////////////////////////////////////
// UPDATE DISPLAY
////////////////////////////////////////////////////////////////////////////////
void DDisplay::update()
{
	m_window->display();
}

////////////////////////////////////////////////////////////////////////////////
// CLEAR DISPLAY
////////////////////////////////////////////////////////////////////////////////
void DDisplay::clear(float _red, float _green, float _blue, float _alpha ) 
{
	glClearColor(_red, _green, _blue, _alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


////////////////////////////////////////////////////////////////////////////////
// ACTIVATE CONTEXT OF WINDOW
////////////////////////////////////////////////////////////////////////////////
void DDisplay::makeActive(bool _a) 
{
	m_window->setActive(_a);
}

////////////////////////////////////////////////////////////////////////////////
// GET WINDOW WIDTH
////////////////////////////////////////////////////////////////////////////////
unsigned int DDisplay::getWidth() const
{
	return m_window->getSize().x;
}

////////////////////////////////////////////////////////////////////////////////
// GET WINDOW HEIGHT
////////////////////////////////////////////////////////////////////////////////
unsigned int DDisplay::getHeight() const
{
	return m_window->getSize().y;
}

////////////////////////////////////////////////////////////////////////////////
// GET POINTER TO WINDOW
////////////////////////////////////////////////////////////////////////////////
sf::Window* DDisplay::getWindowPtr() 
{ 
	return m_window;
}

////////////////////////////////////////////////////////////////////////////////
// POLL EVENTS
////////////////////////////////////////////////////////////////////////////////
void DDisplay::pollEvents()
{
	sf::Event e;
	while ( m_window->pollEvent(e) )
	{
		if (m_paused)
		{
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Right)
			{
				m_paused = false;
				m_window->setMouseCursorVisible(false);
				//this->setMouseCenter();
			}
		}
		else 
		{
			switch (e.type)
			{

				case sf::Event::KeyPressed:
					if (e.key.code == sf::Keyboard::Escape)
						m_running = false;
					// else if (e.key.code == sf::Keyboard::F)
					// {
					// 	wire = !wire;	
					// 	std::cout << "SET: " << (wire ? "true" : "false") << std::endl;
					// }
					break;


	//			case sf::Event::MouseMoved:
	//				input.mouseX = sf::Mouse::getPosition(*m_window).x;
	//				input.mouseY = sf::Mouse::getPosition(*m_window).y;
	//				
	//				break;

				case sf::Event::MouseButtonPressed:
					if (e.mouseButton.button == sf::Mouse::Button::Right)
					{
						m_paused = true;
						m_window->setMouseCursorVisible(true);
					}
				break;
		

				default:
					break;

			}
		}
		
		
	}
}

// PRIVATE
//------------------------------------------------------------------------------
//void Display::pollEvent(float delta_time)
//{
//	sf::Event e;
//	while ( m_window->pollEvent(e) )
//	{
//		int deltaX, deltaY;
//		sf::Vector2i pos ;
//		if (m_paused)
//		{
//			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Right)
//			{
//				m_paused = false;
//				//m_window->setMouseCursorVisible(false);
//				this->setMouseCenter();
//			}
//		}
//		else 
//		{
//			switch (e.type)
//			{
//
//				case sf::Event::KeyPressed:
//					if (e.key.code == sf::Keyboard::Escape)
//						m_running = false;
//					else if (e.key.code == sf::Keyboard::W)
//						camera->getTransform()->moveForward(CAM_SPEED * delta_time);
//					else if (e.key.code == sf::Keyboard::S)
//						camera->getTransform()->moveForward(-CAM_SPEED * delta_time);
//					else if (e.key.code == sf::Keyboard::A)
//						camera->getTransform()->moveRight(CAM_SPEED * delta_time);
//					else if (e.key.code == sf::Keyboard::D)
//						camera->getTransform()->moveRight(-CAM_SPEED * delta_time);
//					break;
//
//
//				case sf::Event::KeyReleased:
//					if (e.key.code == sf::Keyboard::W)
//						input.mv[0] = false;
//					else if (e.key.code == sf::Keyboard::S)
//						input.mv[1] = false;
//					else if (e.key.code == sf::Keyboard::A)
//						input.mv[2] = false;
//					else if (e.key.code == sf::Keyboard::D)
//						input.mv[3] = false;
//					break;
//
//				case sf::Event::MouseMoved:
//					pos = sf::Mouse::getPosition(*m_window);
//										
//					 deltaX =  input.centerX - pos.x;
//					deltaY =  input.centerY - pos.y;
//					
//					if (deltaY != 0)
//						camera->getTransform()->rotate(glm::vec3(0,1,0), deltaX * MOUSE_SEN  );
//					if (deltaX != 0)
//						camera->getTransform()->rotate(camera->getTransform()->getRight(), deltaY *  MOUSE_SEN );
//
//		
//					sf::Mouse::setPosition(sf::Vector2i(input.centerX , input.centerY ), *m_window);
//					break;
//
//				case sf::Event::MouseButtonPressed:
//					if (e.mouseButton.button == sf::Mouse::Button::Right)
//					{
//						m_paused = true;
//						m_window->setMouseCursorVisible(true);
//					}
//				break;
//					
//
//				case sf::Event::Resized:
//				#ifdef GLEW
//					glViewport(0,0, e.size.width, e.size.height);
//					this->setMouseCenter();
//				#else
//					gl3wViewport(0,0, e.size.width, e.size.height);
//				#endif // end of GLEW
//
//					break;
//
//				default:
//					break;
//
//			}
//		}
//		
//		
//	}
//}
//	
//
//
//
////inline void update()
////inline void Display::clear()
////inline void clear(float _red, float _green, float _blue, float _alpha )
////inline bool isClosed()

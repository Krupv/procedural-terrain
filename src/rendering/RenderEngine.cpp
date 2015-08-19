#include "RenderEngine.hpp"

#include "Display.hpp"

#include "../entity/IEntity.hpp"
#include "../entity/FreeMove.hpp"
#include "../entity/FreeLook.hpp"

#include <SFML/Window/Keyboard.hpp> //for wireframe




// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
RenderingEngine::RenderingEngine (std::shared_ptr<IEntity>& _root, int _width, int _height, const std::string& _title )
{
	m_window = std::make_unique<DDisplay>(_width, _height, _title);

	m_root = _root;
	m_wireFrame = false;
	
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
RenderingEngine::~RenderingEngine ()
{
	if (m_thread)
		m_thread->join();
}

////////////////////////////////////////////////////////////////////////////////
// CHECK IF DISPLAY IS RUNNING
////////////////////////////////////////////////////////////////////////////////
bool RenderingEngine::isRunning() 
{ 
	return m_window->isRunning(); 
}

////////////////////////////////////////////////////////////////////////////////
// POLL DISPLAY EVENTS
////////////////////////////////////////////////////////////////////////////////
void RenderingEngine::pollEvents() 
{ 
	m_window->pollEvents();
}

////////////////////////////////////////////////////////////////////////////////
// START THREAD
////////////////////////////////////////////////////////////////////////////////
void RenderingEngine::startThreaded (  )
{
	m_window->makeActive(true);

	if ( ! m_thread)
		m_thread = std::make_unique<std::thread>(&RenderingEngine::run, this);

}

////////////////////////////////////////////////////////////////////////////////
// ADD CAMERA
////////////////////////////////////////////////////////////////////////////////
void RenderingEngine::addCamera()
{
	m_cam = new Camera((float) m_window->getWidth() / (float) m_window->getHeight(), 70.f, 0.5f, 100000.f);
	m_cam->setPos( glm::vec3 (10000.f, 0.f, 10000.f));

	addEntity(m_cam);
}

////////////////////////////////////////////////////////////////////////////////
// GET MAIN CAMERA
////////////////////////////////////////////////////////////////////////////////
Camera* RenderingEngine::getCamera() 
{
	return m_cam;
}

////////////////////////////////////////////////////////////////////////////////
// GET WINDOW SIZE
////////////////////////////////////////////////////////////////////////////////
glm::vec2 RenderingEngine::getWindowSize()
{
	return glm::vec2( m_window->getWidth(), m_window->getHeight());

}

////////////////////////////////////////////////////////////////////////////////
// STEP
////////////////////////////////////////////////////////////////////////////////
// CAREFULL: will throw error if you render in one thread and try to step from other
void RenderingEngine::step (  )
{
	#ifdef DEBUG
		if (m_thread)
			assert ( std::this_thread::get_id() == m_thread->get_id() && "Rendering is already running in its own thread" );
	#endif // end of DEBUG

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
		 m_wireFrame = !m_wireFrame;

	if (m_wireFrame)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


	Tmatricies pipe;
	pipe.view = m_cam->getView();
	pipe.projection = m_cam->getProjection();

	m_window->clear(0.5, 0.6, 0.7, 1.0);

	m_root->renderAll(pipe);
	
	m_window->update();
}


////////////////////////////////////////////////////////////////////////////////
// ADD VISUAL ENTITY
////////////////////////////////////////////////////////////////////////////////
void RenderingEngine::addEntity(IEntity* _child, IEntity* _parent)
{
	if (_parent == nullptr)
	{
		_parent = m_root.get();
		_parent->addChild(_child);
		_child->setTransParent(nullptr);
	}
	else
		_parent->addChild(_child);

}




// PRIVATE
//------------------------------------------------------------------------------
//#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// RUN
////////////////////////////////////////////////////////////////////////////////
void RenderingEngine::run (  )
{
 	m_window->makeActive(true);

	int frame = 0;
	auto start = std::chrono::high_resolution_clock::now();
	auto t1 = std::chrono::high_resolution_clock::now();

	float min_time = 1E9f/60.f;


	while (m_window->isRunning())
	{
		while (m_window->isPaused())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}




		step();
		t1 = std::chrono::high_resolution_clock::now();
		float delay = std::chrono::duration_cast<std::chrono::nanoseconds>(start - t1).count();


		if (delay < min_time)
			std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(min_time - delay)));

		
		 // if ( (frame++ % 60) == 0)
		 // {
		 // 	auto dur = t1 - start;
		 // 	auto durSec = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
		 // 	//std::cout << std::to_string((int) (frame / durSec.count())) << " " << std::flush;
		 // 	logmsg(std::to_string((int) (frame / durSec.count())));
		 // 	//std::cout << "\r";// << std::endl;
		 // 	frame = 0;
		 //    start = std::chrono::high_resolution_clock::now();
		 // }
		start = std::chrono::high_resolution_clock::now();

		

	}
}

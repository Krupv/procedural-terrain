#include "CoreEngine.hpp"
#include "utils/Lua.hpp" //loading config from lua script


// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
CoreEngine::CoreEngine ( const std::string& _confFile )
{

#ifdef TARGET_LINUX
	XInitThreads();
	debug("Running multithreaded");
#endif

	m_root = std::make_shared<IEntity>();
	m_lua = std::make_unique<Lua>( _confFile );

	int w = m_lua->getInt("window.w", 1024);
	int h = m_lua->getInt("window.h", 768);
	std::string title = m_lua->getStr("window.title", "procedural terrain generation");

	m_renEngine = std::make_unique<RenderingEngine>(  m_root, w, h, title  );

	debug("width = " + std::to_string(w) + std::string(" height ") + std::to_string(h));
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
CoreEngine::~CoreEngine ()
{
}

////////////////////////////////////////////////////////////////////////////////
// START CORE ENGINE
////////////////////////////////////////////////////////////////////////////////
void CoreEngine::start()
{
	//m_renEngine->startThreaded();

	while (m_renEngine->isRunning())
	{
		m_renEngine->pollEvents();
		this->update();
		m_renEngine->step();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}

}

////////////////////////////////////////////////////////////////////////////////
// ADD ENTITY TO ENGINE
////////////////////////////////////////////////////////////////////////////////
void CoreEngine::addEntity(IEntity* _child, IEntity* _parent )
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

////////////////////////////////////////////////////////////////////////////////
// STEP CORE ENGINE
////////////////////////////////////////////////////////////////////////////////
void CoreEngine::update()
{
	// float delta = std::chrono::duration<float>(m_timer - now).count();
	//m_root->updateAll(delta);
	auto now = Clock::now();
	m_root->updateAll(m_timer);
	m_timer = Clock::now();
}




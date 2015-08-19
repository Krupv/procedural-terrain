#pragma once
#include "main.hpp"

#ifdef TARGET_LINUX
	#include <X11/Xlib.h> 
#endif


#include <memory>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

#include "rendering/RenderEngine.hpp"
// #include "entity/IEntity.hpp"
class IEntity;
class Lua;

#define CONFIG_FILE "config.lua"

class CoreEngine {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
	CoreEngine( const std::string& _confFile = CONFIG_FILE );
	~CoreEngine();

	void start();
	void addEntity(IEntity* _child, IEntity* _parent = nullptr);


	RenderingEngine* getRenderingEngine() { return m_renEngine.get(); };

private:
	// PRIVATE MEMBERS
	std::unique_ptr<RenderingEngine> m_renEngine;
	std::shared_ptr<IEntity> m_root;
	TimePoint m_timer;
	std::unique_ptr<Lua> m_lua;


	// PRIVATE METHODS
	void update();
};


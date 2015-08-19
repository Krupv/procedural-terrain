#pragma once


#include "../main.hpp"

#include <thread>
#include <memory>

#include <glm/glm.hpp>

#include "../entity/Camera.hpp"
//tmp

class IEntity;
class DDisplay;

class RenderingEngine {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
	RenderingEngine( std::shared_ptr<IEntity>& _root, int _width = 1024, int _height = 768, const std::string& _title = "" );
	~RenderingEngine();

	bool isRunning();
	void pollEvents();

	void addEntity(IEntity* _child, IEntity* _parent = nullptr);

	void addCamera();
	Camera* getCamera() ;
	glm::vec2 getWindowSize();

	void startThreaded();
	void step();
private:
	// PRIVATE MEMBERS
	void run();

	//void limitFPS(std::chrono::high_resolution_clock::time_point* _t1);

	// PRIVATE METHODS
	std::unique_ptr <DDisplay> m_window;
	std::unique_ptr <std::thread> m_thread;
	Camera* m_cam;

	//std::unique_ptr<IEntity> m_root;
	std::shared_ptr<IEntity> m_root;

	bool m_wireFrame;
};



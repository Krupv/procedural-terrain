#include "main.hpp"


#include "CoreEngine.hpp"
#include "rendering/BasicShader.hpp"
#include "rendering/PhongShader.hpp"
#include "rendering/BasicTerrainShader.hpp"
#include "rendering/PhongTerrainShader.hpp"
#include "generator/Landscape.hpp"


#include <thread>
#include <string>


#include <glm/glm.hpp>

#include "entity/FreeMove.hpp"
#include "entity/FreeLook.hpp"

#define CAM_SPEED -100.f
#define MOUSE_SEN 0.05f

#include "static_libs/obj_loader.h"



int main(int argc, char* argv[])
{
	CoreEngine engine;
	RenderingEngine* renEngine = engine.getRenderingEngine();

	renEngine->addCamera();

	Camera* cam = renEngine->getCamera();


	std::string terrain = "landscape.lua";
	if (argc > 1)
		terrain = argv[1];

		
	Landscape* land = new Landscape(terrain, cam);

	IShader* sha = new PhongTerrainShader();
	// IShader* sha = new BasicTerrainShader();

	sha->setAmbient( glm::vec3 (0.3,0.3,0.3));
	//sha->setDirLight(DirectionalLight( BaseLight( glm::vec3 (1,0.8,0.7), 1.2f), glm::vec3 (0.3f, -1.f, 0.f)));
	sha->setDirLight(DirectionalLight( BaseLight( glm::vec3 (1,0.8,0.7), 1.2f), glm::vec3 (0.3f, 0.1f, 0.f)));
	land->setShader(sha);

	glm::vec2 screen = renEngine->getWindowSize();
	cam->addComponent( new FreeLook(MOUSE_SEN, sf::Vector2i(screen.x * 0.5, screen.y * 0.5)));
	cam->addComponent( new FreeMove(CAM_SPEED, *land->getNoiseGenerator()));


	engine.start();
    return 0;
}


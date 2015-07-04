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

#define CAM_SPEED -50.f
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

	//tmp
	// canyon
	//cam->setPos( glm::vec3 (-23005.4f,28495.4f,13870.1f));
	// landscape
	//cam->setPos( glm::vec3 (-90564.7f,14368.7,-29590.1f));
	// desert
	//cam->setPos( glm::vec3 (-90564.7f,-5000.7,-29590.1f));
	//cam->rotate(glm::vec3(0,0,1), 50.f);
	//cam->rotate(glm::vec3(0,1,0), 28.f);
	//cam->rotate(glm::vec3(1,0,0), -90.f);
		
	Landscape* land = new Landscape(terrain, cam);

	
	//cam->setPos( glm::vec3 (-90564.7f,-50000.7,-29590.1f));

	IShader* sha = new PhongTerrainShader();
	// IShader* sha = new BasicTerrainShader();
	sha->setAmbient( glm::vec3 (0.3,0.3,0.3));
	//sha->setDirLight(DirectionalLight( BaseLight( glm::vec3 (1,0.8,0.7), 1.2f), glm::vec3 (0.3f, -1.f, 0.f)));
	sha->setDirLight(DirectionalLight( BaseLight( glm::vec3 (1,0.8,0.7), 1.2f), glm::vec3 (0.3f, 0.1f, 0.f)));
	land->setShader(sha);

	glm::vec2 screen = renEngine->getWindowSize();
	cam->addComponent( new FreeLook(MOUSE_SEN, sf::Vector2i(screen.x * 0.5, screen.y * 0.5)));
	cam->addComponent( new FreeMove(CAM_SPEED, *land->getNoiseGenerator()));

	// Texture lowTex(std::string(RESOURCE_FOLDER) + std::string("images/grass5.jpg"), GL_LINEAR_MIPMAP_LINEAR);
	// Texture midTex(std::string(RESOURCE_FOLDER) + std::string("images/rock3.jpg"), GL_LINEAR_MIPMAP_LINEAR);
	// Texture highTex(std::string(RESOURCE_FOLDER) + std::string("images/rock4.jpg"), GL_LINEAR_MIPMAP_LINEAR);
    //
    //
	// Material mat( glm::vec3 (1,1,1), 20.f);
	// mat.addTexture(lowTex);
	// mat.addTexture(midTex);
	// mat.addTexture(highTex);
    //
	// land->setMaterial(mat);

	// engine.addEntity(land, engine.getMainCamera());
	// land->setTransParent(nullptr);

	// float aa = 10.f;
	// std::thread aaa(&Landscape::updateaa, land, aa);
	// aaa.detach();

	engine.start();
    return 0;
}


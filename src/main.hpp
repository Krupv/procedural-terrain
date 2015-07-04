#pragma once
#ifdef CLANG_COMPLETE_ONLY
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

#define GLM_FORCE_RADIANS
#define THREAD_SAFE

#include "debug/Debug.hpp"
#define error(x) error_print( x , __func__, __FILE__, __LINE__)
#define glError() gl_error_print( __func__, __FILE__, __LINE__)
#define debug(x) debug_print( x , __func__, __FILE__, __LINE__)

#include "debug/Logger.hpp"
#define logmsg(x) Logger::write_log( x );


#ifndef RESOURCE_FOLDER
	#define RESOURCE_FOLDER "resources/"
#endif

#ifndef TEXTURE_FOLDER
	#define TEXTURE_FOLDER "resources/images/"
#endif

#ifndef MESH_FOLDER
	#define MESH_FOLDER "resources/models/"
#endif










#ifdef JUNK

volatile bool wire = false;

typedef struct iin {
	DDisplay* display;
	Camera* camera;
} Tiin;

void renderingTh(Tiin* t)
{
	t->display->makeActive(true);

	glm::vec3 dir = glm::vec3 (-0.7,-0.8,0.5);
	PhongShader skyShader;
	skyShader.setAmbient( glm::vec3 (0.1, 0.1, 0.1));
	skyShader.setDirLight( DirectionalLight (BaseLight( glm::vec3 (1.f,1.f,1.f), 2.0f), dir ));
	
	PhongTerrainShader shader;
	shader.setAmbient( glm::vec3 (0.1, 0.1, 0.1));
	shader.setDirLight( DirectionalLight( BaseLight( glm::vec3 (1.f,1.f,1.f), 1.5f), dir ));
	
	Texture* tex[3];
	tex[0] = new Texture(std::string(RESOURCE_FOLDER) + std::string("images/grass5.jpg"), GL_LINEAR);
	tex[1] = new Texture(std::string(RESOURCE_FOLDER) + std::string("images/rock3.jpg"), GL_LINEAR_MIPMAP_LINEAR);
	tex[2] = new Texture (std::string(RESOURCE_FOLDER) + std::string("images/rock4.jpg"), GL_LINEAR);

	Material terrainMaterial( glm::vec3 (1,1,1), 4.f);
	for ( int i = 0; i < 3 ; i++ )
	{
		terrainMaterial.addTexture(tex[i]);
	}

	Mesh skydome("resources/models/skydome.obj");
	Transform skyt(	glm::vec3(0,-5,0), glm::quat(0,0,0,1), 5000.f);


	Material skyMaterial( glm::vec3 (1,1,1), 2.f);
	Texture* tex2 = new Texture(std::string(RESOURCE_FOLDER) + std::string("images/sky.png"));
	skyMaterial.addTexture(tex2);


	float height = 4000.f;
	Generator gen(2000.f, 128, height);


	sf::Clock clock;
	//float old_time = clock.getElapsedTime().asSeconds();
	float frame = 0.0f;

	sf::Clock ftime;
	while(t->display->isRunning())
	{
	//	float delta_time;

	//	delta_time = clock.getElapsedTime().asSeconds() - old_time;	
	//	old_time = clock.getElapsedTime().asSeconds();

		float elsin = sinf(clock.getElapsedTime().asSeconds() * 0.2f);
		float elcos = cosf(clock.getElapsedTime().asSeconds() * 0.2f);
		 dir = glm::vec3 ( -elsin , -elcos , 0);

		 shader.setDirLight( DirectionalLight( BaseLight( glm::vec3 (0.8,0.8,0.8), 2.f ), dir ));
		 glm::vec3 ddir = dir * -1.f;
		 skyShader.setDirLight( DirectionalLight( BaseLight( glm::vec3 (0.8,0.8,0.8), 2.f ), ddir ));

		t->display->clear(0.5, 0.6, 0.7, 1.0);

        /* Render here */

		stimer("Prepare draw");	
		glm::vec3 camPos = t->camera->getAbsolutePos();
		Tmatricies pipe;
		pipe.view = t->camera->getView();
		pipe.projection = t->camera->getProjection();


		std::vector<glm::mat4> model;
		for (int i = 0; i < gen.counter; i++)
		{
			model.push_back(gen.m_chList[i]->getTransform().getTransMat());
		}

		//skydome
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		skyt.setPos( camPos - glm::vec3(0,2000,0));
		pipe.model = skyt.getTransMat();
		skyShader.bind();
		skyShader.updateUniforms( pipe, skyMaterial);
		skydome.draw();
		glBindTexture(GL_TEXTURE_2D, 0);
		//glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		skyShader.unbind();


		etimer("Prepare draw");	
		if (wire)
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        //
		// std::cout << "wire received " << ((wire) ? "true" : "false") << std::endl;
		stimer("Draw");	


		shader.bind();
		for (int i = 0; i < gen.counter; i++)
		{
			pipe.model = model[i];
			shader.updateUniforms(  pipe, terrainMaterial);
			gen.m_chList[i]->m_mesh->draw();
		}
		etimer("Draw");	

		glm::vec3 plPos = t->camera->getAbsolutePos();
		gen.generateChunkFromPos(plPos.x, plPos.z);

		frame++;
		if (ftime.getElapsedTime().asSeconds() >= 1.f)
		{
			std::cout << "fps: " << frame << std::endl;
			frame = 0;
			ftime.restart();
			
		}
		t->display->update();
	}


}
void aa(IEntity* ent)
{
	while(1)
	{
		ent->updateAll(0.001f);

	}

}

int main(int argc, char* argv[])
{
#ifdef PERLIN
	std::cout << "using perling noise" << std::endl;
#else 
	std::cout << "using simplex noise " << std::endl;
#endif // end of MYNOISE

	XInitThreads();

	 CoreEngine engine;

	// unsigned int w,h;
	// //DDisplay* display;
	// if (argc == 2)
	// {
	// 	w = atoi(argv[1]);
	// 	h = w * (9.f / 16.f);
	// }
	// else if (argc == 3)
	// {
	// 	w = atoi(argv[1]);
	// 	h = atoi(argv[2]);
	// }
	// else
	// {
	// 	w = 800;
	// 	h = 800;
	// }
	//
	// //RenderingEngine renEngine(w, h, "dimzero");
	//  //DDisplay* display = new DDisplay(w, h, "dimzero");
	//
	// //COMMENT
    //
	//  Camera* camera = new Camera((float) w / (float) h, 70.f, 0.01f, 20000.f );
    // //
	//  MouseLook* mm = new MouseLook(MOUSE_SEN, sf::Vector2i(w * 0.5, h * 0.5));
	//  FreeMove* mmm = new FreeMove(CAM_SPEED);
	//  camera->addComponent( mm);
	//  camera->addComponent( mmm);
    //
	//  IEntity* ent = new IEntity();
	//  ent->addChild(camera);
	//Landscape* land = new Landscape();
	//////land.setParent(&camera);
	////camera->addChild( land);
	////land->setTransParent(nullptr);
	//
	//glm::vec3 dir = glm::vec3 (-0.7,-0.8,0.5);
	//
	//PhongTerrainShader shader;
	//shader.setAmbient( glm::vec3 (0.1, 0.1, 0.1));
	//shader.setDirLight( DirectionalLight( BaseLight( glm::vec3 (1.f,1.f,1.f), 1.5f), dir ));
	//
	//Texture* tex[3];
	//tex[0] = new Texture(std::string(RESOURCE_FOLDER) + std::string("images/grass5.jpg"), GL_LINEAR);
	//tex[1] = new Texture(std::string(RESOURCE_FOLDER) + std::string("images/rock3.jpg"), GL_LINEAR_MIPMAP_LINEAR);
	//tex[2] = new Texture (std::string(RESOURCE_FOLDER) + std::string("images/rock4.jpg"), GL_LINEAR);

	//Material terrainMaterial( glm::vec3 (1,1,1), 4.f);
	//for ( int i = 0; i < 3 ; i++ )
	//{
	//	terrainMaterial.addTexture(tex[i]);
	//}
	//land->setMaterial(terrainMaterial);
	//land->setShader(&shader);
	
	//engine.addEntity(land);
	engine.start();
	
    /* Loop until the user closes the window */
    // while ( renEngine.isRunning())
		//  while (1)
      // {
		//   ent->updateAll(0.01f);
	// 	float delta_time;
    //
	// 	// display->pollEvents();
	// 	//renEngine.pollEvents();
    //
    //
	// 	delta_time = clock.getElapsedTime().asSeconds() - old_time;	
	// 	old_time = clock.getElapsedTime().asSeconds();
    //
	// 	camera.updateAll(delta_time);
    //
    //
	// 	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	// //	//sf::sleep( sf::seconds(0.005f));
    // //
      // }
	// t1.join();
	//delete display;
    return 0;
}

#endif

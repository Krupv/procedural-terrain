#include "Landscape.hpp"


//TMP
#include <SFML/System.hpp>

#include "../rendering/Material.hpp"
#include "../entity/Skydome.hpp"

//TMP
#include <iostream>



// PUBLIC
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Landscape::Landscape ( const std::string& _luaFile , IEntity* _observer ) : IEntity(_observer)
{
	float posX = 0.f;
	float posZ = 0.f;

	m_sunPos = 0.f;

	Lua lua(_luaFile);

	m_cellSize = lua.getFloat("cell.size", DEF_CELL_SIZE);
	m_cellHigh = lua.getFloat("cell.height", DEF_CELL_HEIGHT);
	m_cellResolution = lua.getInt("cell.resolution", DEF_CELL_RES);

	//cell resolution must be even to allow CHunkedLOD optimalizations
	//= copying odd verticies from parent model instead of generating them
	//  with noise generator
	if ((m_cellResolution % 2) != 1)
		m_cellResolution++;

	m_LOD = lua.getInt("cell.LOD", DEF_LOD_LEVEL);

	if (lua.tableExists("material"))
	{
		Texture t1(lua.getStr("material.texture.low", "images/fallback.jpg"), GL_LINEAR_MIPMAP_LINEAR);
		Texture t2(lua.getStr("material.texture.hight", "images/fallback.jpg"), GL_LINEAR_MIPMAP_LINEAR);
		float texScale = lua.getFloat("material.scale", 1.f);
		Material mat( glm::vec3 (1.0, 1.0, 1.0), texScale);
		mat.addTexture(t1);
		mat.addTexture(t2);
		this->setMaterial(mat);
	}

#ifdef PERLIN
	m_noise = new PerlinNoise(_luaFile);
#else 
	m_noise = new ValueNoise(_luaFile);
#endif // end of PERLIN



	if (_observer)
	{
		_observer->addChild(this);
		this->setTransParent(nullptr);
		glm::vec3 pos = _observer->getAbsolutePos();
		posX = pos.x;
		posZ = pos.z;
	}

	m_skydome = new Skydome("dome.obj", "sky.png");
	_observer->addChild(m_skydome); // add skydome under camera
	m_skydome->setTransParent(nullptr); //we dont want the skydome to rotate etc with the camera
										// only to copy its position which solves update func

	
	for ( int ix = 0; ix < LAN_SIZE  ; ix++ )
		for ( int iz = 0; iz < LAN_SIZE  ; iz++ )
		{
			m_terrain[ix][iz] = std::make_unique<TerrainCell>((m_cellSize * ix) + posX, (m_cellSize * iz) + posZ,  m_cellSize,m_cellResolution, m_LOD, m_noise);
				//;
			m_mutexes[ix][iz] = new std::mutex();
		}
	// m_mutex = new std::mutex();
				//m_terrain[1][1] = std::unique_ptr<Chunk>(new Chunk(1, 1, 32, DFLT_HEIGH, DFLT_SIZE ));
	
	runningThreaded = false;
	updateThread = std::thread(&Landscape::updateThreaded, this);
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Landscape::~Landscape ()
{
	runningThreaded = false;
	updateThread.join();
}




// PRIVATE
//------------------------------------------------------------------------------

void Landscape::moveX(float _distance, const glm::vec3& _pos)
{
	if (_distance >= 0.f)
	{
		for ( int iz = 0; iz < m_cellCount ; iz++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[0][iz]);
			m_terrain[0][iz].reset();
		}

		for ( int ix = 0; ix < m_cellCount - 1; ix++ )
			for ( int iz = 0; iz < m_cellCount ; iz++ )
			{
				std::lock_guard<std::mutex> lock(*m_mutexes[ix][iz]);
				std::lock_guard<std::mutex> lock2(*m_mutexes[ix+1][iz]);
				m_terrain[ix][iz] = std::move(m_terrain[ix+1][iz]);
			}

		for ( int iz = 0; iz < m_cellCount ; iz++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[m_cellCount - 1][iz]);
			m_terrain[m_cellCount - 1][iz] = std::make_unique<TerrainCell>(m_terrain[m_cellCount -2][iz]->getAbsolutePos().x + m_cellSize,  m_terrain[m_cellCount -2][iz]->getAbsolutePos().z , m_cellSize, m_cellResolution, m_LOD, m_noise);
			
		}
	}
	else
	{
		for ( int iz = 0; iz < m_cellCount ; iz++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[m_cellCount - 1][iz]);
			m_terrain[m_cellCount - 1][iz].reset();
		}

		for ( int ix =  m_cellCount - 1; ix > 0; ix-- )
			for ( int iz = 0; iz < m_cellCount ; iz++ )
			{
				std::lock_guard<std::mutex> lock(*m_mutexes[ix][iz]);
				std::lock_guard<std::mutex> lock2(*m_mutexes[ix-1][iz]);
				m_terrain[ix][iz] = std::move(m_terrain[ix-1][iz]);
			}

		for ( int iz = 0; iz < m_cellCount ; iz++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[0][iz]);
			m_terrain[0][iz] = std::make_unique<TerrainCell>(m_terrain[1][iz]->getAbsolutePos().x - m_cellSize,  m_terrain[1][iz]->getAbsolutePos().z , m_cellSize, m_cellResolution, m_LOD, m_noise);
		}
	}
}



void Landscape::moveZ(float _distance, const glm::vec3& _pos)
{
	if (_distance >= 0.f)
	{
		for ( int ix = 0; ix < m_cellCount ; ix++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[ix][0]);
			m_terrain[ix][0].reset();
		}

		for ( int ix = 0; ix < m_cellCount; ix++ )
			for ( int iz = 0; iz < m_cellCount - 1; iz++ )
			{
				std::lock_guard<std::mutex> lock(*m_mutexes[ix][iz]);
				std::lock_guard<std::mutex> lock2(*m_mutexes[ix][iz+1]);
				m_terrain[ix][iz] = std::move(m_terrain[ix][iz+1]);
			}

		for ( int ix = 0; ix < m_cellCount ; ix++ )
		{
				std::lock_guard<std::mutex> lock(*m_mutexes[ix][m_cellCount - 1]);
			m_terrain[ix][m_cellCount - 1] = std::make_unique<TerrainCell>(m_terrain[ix][m_cellCount -2]->getAbsolutePos().x,  m_terrain[ix][m_cellCount -2]->getAbsolutePos().z + m_cellSize, m_cellSize , m_cellResolution, m_LOD, m_noise);
		}
	}
	else
	{
		for ( int ix = 0; ix < m_cellCount ; ix++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[ix][m_cellCount - 1]);
			m_terrain[ix][m_cellCount - 1].reset();
		}

		for ( int ix =  0; ix < m_cellCount; ix++ )
			for ( int iz = m_cellCount - 1; iz > 0 ; iz-- )
			{
				std::lock_guard<std::mutex> lock(*m_mutexes[ix][iz]);
				std::lock_guard<std::mutex> lock2(*m_mutexes[ix][iz-1]);
				m_terrain[ix][iz] = std::move(m_terrain[ix][iz-1]);
			}

		for ( int ix = 0; ix < m_cellCount ; ix++ )
		{
			std::lock_guard<std::mutex> lock(*m_mutexes[ix][0]);
			m_terrain[ix][0] = std::make_unique<TerrainCell>(m_terrain[ix][1]->getAbsolutePos().x ,  m_terrain[ix][1]->getAbsolutePos().z - m_cellSize, m_cellSize ,m_cellResolution, m_LOD, m_noise);
		}
	}
}


void Landscape::render(const Tmatricies& _pipe) const
{

	m_skydome->draw(_pipe);

	m_shader->bind();

	//glDisable(GL_CULL_FACE);
	for ( int i = 0; i < m_cellCount; i++ )
		for ( int j = 0; j < m_cellCount; j++ )
		{
			if ( m_terrain[i][j] )
			{
				//TODO array of mutexec for every cell
				std::lock_guard<std::mutex> lock(*m_mutexes[i][j]);
				//m_mutexes[i][j]->lock();
				// if ( ! m_terrain[i][j]->dataRdy )
				// {
				// 	debug("ffuuu");
				// 	break;
				// }

				// if ( ! m_terrain[i][j]->renderingRdy )
				// 	m_terrain[i][j]->createMesh();

				_pipe.model = m_terrain[i][j]->getTransMat();
				
				m_shader->updateUniforms(_pipe, m_material);	
				m_terrain[i][j]->render();
				//m_mutexes[i][j]->unlock();
				
			}

		}
	
	
	

}
void Landscape::step()
{
	glm::vec3 pos = m_parent->getAbsolutePos();
	int center = (int) (m_cellCount / 2.1);

	glm::vec3 centerpos = m_terrain[center][center]->getAbsolutePos();

	for ( int i = 0; i < m_cellCount; i++ )
		for ( int j = 0; j < m_cellCount; j++ )
		{
			m_terrain[i][j]->updateTree(pos);

		}

	float disX = pos.x - centerpos.x;
	float disZ = pos.z - centerpos.z;

	if ( pos.x > (centerpos.x + m_cellSize) || pos.x < centerpos.x)
	{
		moveX(disX, centerpos);
	}

	if ( pos.z > (centerpos.z + m_cellSize) || pos.z < centerpos.z)
	{
		moveZ(disZ, centerpos);
	}
	//m_terrain[1][1]->addLODlevel();
	 

}

void Landscape::updateThreaded()
{
	sf::Clock  clock ;

	while (!runningThreaded)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while(runningThreaded)
	{
		
	// if ( ! m_parent)
	// 	return;
	step();	

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	} 
}

 void Landscape::update(float _delta)
 {
	 runningThreaded = true;
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
	 {
		 m_sunPos += 0.04f;
		float elsin = sinf(m_sunPos);
		float elcos = cosf(m_sunPos);

		 m_shader->getDirLightPtr()->direction =  glm::vec3 (-elsin, -elcos, -elsin);
		 m_skydome->getMaterialPtr()->setColor( glm::vec3 ( -elcos, -elcos, -elcos));
	 }
	 else if (sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Subtract))
	 {
		 m_sunPos -= 0.04f;
		float elsin = sinf(m_sunPos);
		float elcos = cosf(m_sunPos);

		 m_shader->getDirLightPtr()->direction =  glm::vec3 (-elsin, -elcos, -elsin);
		 m_skydome->getMaterialPtr()->setColor( glm::vec3 ( -elcos, -elcos, -elcos));
	 }

 }

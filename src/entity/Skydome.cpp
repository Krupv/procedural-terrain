#include "Skydome.hpp"

#include <string>

#include "../rendering/BasicShader.hpp"
#include "../rendering/Mesh.hpp"
#include "../rendering/Material.hpp"


#define SKYDOME_OFFSET -12000.f


Skydome::Skydome( const std::string& _mesh, const std::string& _texture ) :
	IEntity(nullptr)
{
	m_mesh = std::make_unique<Mesh>(_mesh);
	this->setScale(10000.f);

	m_material = std::make_unique<Material>( glm::vec3 (1,1,1), 1.f);
	m_material->addTexture(Texture(_texture, GL_LINEAR_MIPMAP_LINEAR ));

	m_shader = std::make_unique<BasicShader>();

}

Skydome::~Skydome()
{

}

void Skydome::draw(const Tmatricies& _pipe) const 
{
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	_pipe.model = getTransMat();
	m_shader->bind();
	m_shader->updateUniforms( _pipe, *m_material);
	m_mesh->draw();
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

//void render(const Tmatricies& _pipe) const;
void Skydome::update(float _delta)
{
	if (m_parent)
	{
		glm::vec3 pos = m_parent->getAbsolutePos();

		pos.y += SKYDOME_OFFSET;

		this->setPos(pos);
	}
}


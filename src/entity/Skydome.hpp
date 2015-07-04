#pragma once
#include "../main.hpp"


#include <memory>
#include "IEntity.hpp"

class Material;
class Mesh;
class BasicShader;




class Skydome : public IEntity {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
	Skydome( const std::string& _mesh, const std::string& _texture );
	~Skydome();
	void draw(const Tmatricies& _pipe) const;
	Material* getMaterialPtr() { return m_material.get(); };

protected:

	//void render(const Tmatricies& _pipe) const;
	void update(float _delta);

private:
	// PRIVATE MEMBERS
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<Material> m_material;
	std::unique_ptr<BasicShader> m_shader;
	


	// PRIVATE METHODS
};


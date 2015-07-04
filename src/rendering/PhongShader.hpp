#pragma once
#include "../main.hpp"

#include "IShader.hpp"


class PhongShader : public IShader {
public:
	// PUBLIC METHODS
	PhongShader();
	~PhongShader();

	const glm::vec3 getAmbient() const;
	void setAmbient(const glm::vec3& _color);
	void setDirLight(const DirectionalLight& _dirLight );

	//void updateUniforms(const glm::mat4& _worldMatrix, const glm::mat4& _projectedMatrix, const Material& _material);
	void updateUniforms(const Tmatricies& _matricies, const Material& _material) const;
	//TODO: remove this with rendering engine made with threading in mind

private:
	// PRIVATE MEMBERS
	
	// PRIVATE METHODS
	glm::vec3 m_ambientLight;
	DirectionalLight m_dirLight;
};

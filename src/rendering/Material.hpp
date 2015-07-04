#pragma once
#include "../main.hpp"

#include <vector>
#include <glm/glm.hpp>

#include "Texture.hpp"


class Material {
public:

	// PUBLIC METHODS
	Material(const glm::vec3& _color = glm::vec3(1,1,1), const float& _texScale = 1.f);
	Material(const Material& _other);
	~Material();

	const glm::vec3& getColor() const { return m_color; };
	float getTexScale() const { return m_texScale; };

	void setTexScale(float _scale) { m_texScale = _scale; };

	void addTexture(Texture _texture);
	void bindTextures() const;

	void setColor( const glm::vec3& _color)
	{
		m_color = _color;

	}

private:
	// PRIVATE MEMBERS
	std::vector<Texture> m_textures;
	glm::vec3 m_color;
	float m_texScale;
	
	// PRIVATE METHODS
};

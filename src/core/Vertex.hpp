#pragma once
#include "../main.hpp"

#include <glm/glm.hpp>

class Vertex {
public:
	Vertex(const glm::vec3& _pos, const glm::vec2& _texCoord, const glm::vec3& _normal = glm::vec3(0,0,0));
	~Vertex();

	glm::vec3* getPos();
	glm::vec2* getTexCoords() ;
	glm::vec3* getNormal()	;

private:
	glm::vec3 m_pos;
	glm::vec2 m_texCoord;
	glm::vec3 m_normal;
}; // end of class


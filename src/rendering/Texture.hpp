#pragma once
#include "../main.hpp"

#include <memory>
#include <map>

#include <GL/glew.h> //Glfloat, GLenum etc..


typedef struct textureData {
	textureData( const std::string& _name) : m_fileName( _name) {};
	~textureData() 
	{
	  glDeleteTextures(1, &textureID);
	};

	std::string m_fileName;
	GLuint textureID;
	GLenum textureTarget;
} TextureData;


class Texture   {

public:
	Texture(const std::string& _filename, GLfloat _filter = GL_LINEAR, GLenum _texTarget = GL_TEXTURE_2D);
	Texture(const Texture& _other);
	~Texture();

	void bind(unsigned int _unit) const;

private:
	static std::map < std::string, std::shared_ptr<TextureData>> m_resourceMap;
	std::shared_ptr<TextureData> m_data;
};



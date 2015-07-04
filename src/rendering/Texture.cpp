#include "Texture.hpp"

#include <cassert>
#include <string>

#include <SFML/Graphics/Image.hpp>


std::map < std::string, std::shared_ptr<TextureData>> Texture::m_resourceMap;

// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Texture::Texture(const std::string& _filename, GLfloat _filter, GLenum _texTarget)
{
	const std::string fileName = TEXTURE_FOLDER + _filename;
	m_data = m_resourceMap[fileName];

	if (m_data == nullptr)
	{
		debug("creating resource :" + fileName);

		m_data = std::make_shared<TextureData>(fileName);
		m_resourceMap[_filename] =  m_data;

		m_data->textureTarget = _texTarget;
		sf::Image img_data;
		if (!img_data.loadFromFile(fileName))
		{
			error("Could not load \"" +fileName + "\" texture");
			return;
		}

		glEnable(m_data->textureTarget);
		glGenTextures(1, &m_data->textureID);
		glBindTexture(m_data->textureTarget, m_data->textureID);

		glTexParameteri(m_data->textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_data->textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(m_data->textureTarget, GL_TEXTURE_MIN_FILTER, _filter);
		glTexParameterf(m_data->textureTarget, GL_TEXTURE_MAG_FILTER, _filter);

		glTexImage2D(m_data->textureTarget, 0, GL_RGBA8, img_data.getSize().x, img_data.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());

		if (_filter == GL_NEAREST_MIPMAP_NEAREST || _filter == GL_NEAREST_MIPMAP_LINEAR ||
				_filter == GL_LINEAR_MIPMAP_NEAREST || GL_LINEAR_MIPMAP_LINEAR )
		{
			GLfloat maxAnisotropy;
			glGenerateMipmap(m_data->textureTarget);
			glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(m_data->textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		}
		else
		{
			glTexParameteri(m_data->textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_data->textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}

	}




		
}

////////////////////////////////////////////////////////////////////////////////
// COPY CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Texture::Texture(const Texture& _other) : 
	m_data(_other.m_data)
{

}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
	if (m_resourceMap[m_data->m_fileName].use_count() <= 2) // 2 means that only this texture and resourceMap hold shared pointer to texture
	{
		m_resourceMap.erase(m_data->m_fileName);
	}

}

////////////////////////////////////////////////////////////////////////////////
// BIND TEXTURE
////////////////////////////////////////////////////////////////////////////////
void Texture::bind(unsigned int _unit) const
{
	assert(_unit <= 31);
	//glEnable(m_data->textureTarget);
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(m_data->textureTarget, m_data->textureID);
}



// PRIVATE
//------------------------------------------------------------------------------




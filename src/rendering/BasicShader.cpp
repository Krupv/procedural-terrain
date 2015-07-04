#include "BasicShader.hpp"



// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
BasicShader::BasicShader() : IShader( SHADER_FILE_BASIC )
{
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".vs", E_ST_Vertex);
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".fs", E_ST_Fragment);

	bindAttribLocation("position", 0);
	bindAttribLocation("texCoord", 1);

	linkProgram( m_data->program );

	addUniform("modelViewProjection");
	addUniform("baseColor");
	addUniform("texScale");
}


////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
BasicShader::~BasicShader()
{
}


////////////////////////////////////////////////////////////////////////////////
// UPDATE ALL UNIFORMS IN SHADER
////////////////////////////////////////////////////////////////////////////////
void BasicShader::updateUniforms(const Tmatricies& _matricies, const Material& _material) const
{
	_material.bindTextures();


	glm::mat4 MVP = _matricies.projection * _matricies.view * _matricies.model;

	setUniform("modelViewProjection", MVP);
	setUniform("baseColor", _material.getColor());
	setUniformf("texScale", _material.getTexScale());
}



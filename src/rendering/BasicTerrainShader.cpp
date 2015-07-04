#include "BasicTerrainShader.hpp"



// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
BasicTerrainShader::BasicTerrainShader() : IShader( SHADER_FILE_BASE_TERR )
{
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".vs", E_ST_Vertex);
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".fs", E_ST_Fragment);

	bindAttribLocation("position", 0);
	bindAttribLocation("texCoord", 1);
	bindAttribLocation("normal", 2);

	linkProgram( m_data->program );

	addUniform("modelViewProjection");
	addUniform("model");
	addUniform("baseColor");
	addUniform("texScale");

	addUniform("terrTexture0");
	addUniform("terrTexture1");
}


////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
BasicTerrainShader::~BasicTerrainShader()
{
}


////////////////////////////////////////////////////////////////////////////////
// UPDATE ALL UNIFORMS IN SHADER
////////////////////////////////////////////////////////////////////////////////
void BasicTerrainShader::updateUniforms(const Tmatricies& _matricies, const Material& _material) const
{
	glm::mat4 MVP = _matricies.projection * _matricies.view * _matricies.model;


	_material.bindTextures();

	setUniform("modelViewProjection", MVP);
	setUniform("model", _matricies.model);

	setUniform("baseColor", _material.getColor());
	setUniformf("texScale", _material.getTexScale());

	setUniformi("terrTexture0", 0);
	setUniformi("terrTexture1", 1);
}


////////////////////////////////////////////////////////////////////////////////
// UPDATE ALL UNIFORMS IN SHADER
////////////////////////////////////////////////////////////////////////////////
//void BasicTerrainShader::updateUniforms( const Transform& _worldTransformation, const Camera& _camera)
//{
//	glm::mat4 MVP = _camera.getViewProjection() * _worldTransformation.getTransMat();
//	setUniform("MVP", MVP);
//}

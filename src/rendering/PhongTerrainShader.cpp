#include "PhongTerrainShader.hpp"



// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
PhongTerrainShader::PhongTerrainShader() : IShader( SHADER_FILE_PHONG_TERR )
{
	m_ambientLight = glm::vec3 (1,1,1);
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".vs", E_ST_Vertex);
	addAndCompileShader( std::string(RESOURCE_FOLDER) + "shaders/" + m_data->fileName + ".fs", E_ST_Fragment);

	bindAttribLocation("position", 0);
	bindAttribLocation("texCoord", 1);
	bindAttribLocation("normal", 2);

	linkProgram( m_data->program );

	addUniform("modelViewProjection");
	addUniform("model");
	addUniform("baseColor");
	addUniform("ambientLight");
	addUniform("texScale");

	addUniform("directionalLight.base.color");
	addUniform("directionalLight.base.intensity");
	addUniform("directionalLight.direction");

	addUniform("terrTexture0");
	addUniform("terrTexture1");
}


////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
PhongTerrainShader::~PhongTerrainShader()
{
}


////////////////////////////////////////////////////////////////////////////////
// GET AMBIENT LIGHT COLOR
////////////////////////////////////////////////////////////////////////////////
const glm::vec3 PhongTerrainShader::getAmbient() const
{
	return m_ambientLight;
}


////////////////////////////////////////////////////////////////////////////////
// SET AMBIENT LIGHT COLOR
////////////////////////////////////////////////////////////////////////////////
void PhongTerrainShader::setAmbient(const glm::vec3& _color)
{
	m_ambientLight = _color;
}


void PhongTerrainShader::setDirLight(const DirectionalLight& _dirLight )
{
	m_dirLight = _dirLight;
}


////////////////////////////////////////////////////////////////////////////////
// UPDATE ALL UNIFORMS IN SHADER
////////////////////////////////////////////////////////////////////////////////
void PhongTerrainShader::updateUniforms(const Tmatricies& _matricies, const Material& _material) const
{
	_material.bindTextures();

	glm::mat4 modelView = _matricies.view * _matricies.model;

	setUniform("modelViewProjection", _matricies.projection * modelView);
	setUniform("model", _matricies.model);
	setUniform("baseColor", _material.getColor());
	setUniform("ambientLight", m_ambientLight);
	setUniformf("texScale", _material.getTexScale());

	setUniform("directionalLight", m_dirLight);

	setUniformi("terrTexture0", 0);
	setUniformi("terrTexture1", 1);
}



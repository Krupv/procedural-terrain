#pragma once
#include "../main.hpp"

#include <memory>
#include <string> //for shader file name
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Material.hpp"


typedef struct matrices 
{
	mutable glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

} Tmatricies;

struct  BaseLight 
{
	glm::vec3 color;
	float intensity;

	BaseLight( const glm::vec3& _color = glm::vec3 (0,0,0), float _intensity = 0.f) 
	{
		color = _color;
		intensity = _intensity;
	}
};

struct DirectionalLight 
{
	BaseLight base;
	glm::vec3 direction;

	DirectionalLight(const BaseLight& _base = BaseLight(), const glm::vec3& _direction = glm::vec3 (0,-1,0)) : base(_base)
	{
		direction = glm::normalize(_direction);
	}
};


#define ERR_MSG_SIZE 1024 //size for shader error message buffer

#define SHADER_FOLDER #RESOURCES_FOLDER#/shaders
#define SHADER_FILE_BASIC "basicShader"
#define SHADER_FILE_PHONG "phongShader"
#define SHADER_FILE_BASE_TERR "basicTerrain"
#define SHADER_FILE_PHONG_TERR "phongTerrain"


enum E_UnitType { E_UT_Int, E_UT_Float, E_UT_Vec3, E_UT_Mat4 };
enum E_ShaderType { E_ST_Vertex = 0, E_ST_Fragment, E_ST_NumShaders };

typedef struct shaderData {
	shaderData() 
	{
		program = 0;
		for ( int i = 0; i < E_ST_NumShaders; i++ )
			shaders[i] = 0;
	}

	GLuint			program;
	GLuint			shaders[ E_ST_NumShaders ];
	std::string		fileName;

} ShaderData;


class IShader {
public:

	// PUBLIC METHODS
	IShader(const std::string& _fileName);
	~IShader();

	virtual void updateUniforms(const Tmatricies& _matricies, const Material& _material) const = 0 ;


	void bind();
	void unbind()
	{
		glUseProgram(0);
	};

	void addAndCompileShader(const std::string& _fileName,	const E_ShaderType _shaderType	);

	void addUniform (const std::string& _uniform);

	void setUniformi(const std::string& _uniformName,	int _value					) const ; 
	void setUniformf(const std::string& _uniformName,	float _value				) const ;
	void setUniform	(const std::string& _uniformName,	glm::vec3 _value			) const ;
	void setUniform	(const std::string& _uniformName,	glm::mat4 _value			) const ;
	void setUniform	(const std::string& _uniformName,	BaseLight _baseLight		) const;
	void setUniform	(const std::string& _uniformName,	DirectionalLight _dirLight	) const;

	virtual void setAmbient(const glm::vec3& ) {};
	virtual void setDirLight(const DirectionalLight& ) {};
	virtual DirectionalLight* getDirLightPtr() { return nullptr; };

protected:
	// PRIVATE MEMBERS
	static std::map< std::string, std::shared_ptr<ShaderData>> m_resourceMap;

	std::shared_ptr<ShaderData> m_data;
	mutable std::map< const std::string, GLuint>	m_uniforms;
	
	// PRIVATE METHODS
	GLuint		createShader		(const std::string&		_shaderSource,	E_ShaderType shaderType	);
	std::string loadShader			(const std::string&		_fileName								);
	void		bindAttribLocation	(const GLchar*			_attrib,		unsigned int _location	);
	void		linkProgram			(GLuint					_program								);
	void		checkShaderError	(GLuint					_shader,		GLuint _flag,		bool _isProgram,	const std::string& _errorMessage);
};

#include "IShader.hpp"

#include <fstream> //for opening shader file

#include <glm/gtc/type_ptr.hpp>

std::map< std::string, std::shared_ptr<ShaderData>> IShader::m_resourceMap;

// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
IShader::IShader (const std::string& _fileName ) 
{
	m_data = m_resourceMap[_fileName];
	if (m_data == nullptr)
	{
		m_data = std::make_shared<ShaderData>();
		m_data->fileName = _fileName;
		m_resourceMap[_fileName] = m_data;
		m_data->program = glCreateProgram();
		if ( m_data->program == 0)
		{
			error(std::string("could not create program named: ") + _fileName);
			return;
		}
	}

}


////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
IShader::~IShader ( )
{
	for ( unsigned int i = 0; i < E_ST_NumShaders; i++ )
	{
		glDetachShader(m_data->program, m_data->shaders[i]);
		glDeleteShader(m_data->shaders[i]);
	}
	
	glDeleteProgram(m_data->program);
}


////////////////////////////////////////////////////////////////////////////////
// BIND SHADER
////////////////////////////////////////////////////////////////////////////////
void IShader::bind()
{
	glUseProgram(m_data->program);
}


////////////////////////////////////////////////////////////////////////////////
// ADD NEW SHADER
////////////////////////////////////////////////////////////////////////////////
void IShader::addAndCompileShader(const std::string& _fileName, const E_ShaderType _shaderType)
{
	if (m_data->shaders[_shaderType] == 0)
	{
		m_data->shaders[_shaderType] = createShader(loadShader(_fileName), _shaderType);
		glAttachShader(m_data->program, m_data->shaders[_shaderType]);
	}
}




//void Shader::update(const std::string& _uniformName, const Transform& _transform, const Camera& _camera)
//{
//
//	glm::mat4 projectedMatrix = _camera.getViewProjection() * _transform.getTransMat();
//	//glUniformMatrix4fv(m_uniforms[E_UNIFORM_TRANSFORM], 1, GL_FALSE, &transform[0][0]);
////	glUniformMatrix4fv(m_uniforms[_uniformName], 1, GL_FALSE, &projectedMatrix[0][0]);
//	setUniform(_uniformName, projectedMatrix);
//}
//
//
//void Shader::update(const std::string& _uniformName, const Transform& _transform, const glm::mat4& _projection)
//{
//	glm::mat4 projectedMatrix = _projection * _transform.getTransMat();
//	//glUniformMatrix4fv(m_uniforms[E_UNIFORM_TRANSFORM], 1, GL_FALSE, &transform[0][0]);
////	glUniformMatrix4fv(m_uniforms[_uniformName], 1, GL_FALSE, &projectedMatrix[0][0]);
//	setUniform(_uniformName, projectedMatrix);
//}
//void Shader::update(const std::string& _uniformName, const E_uniType _uniType, void* _value )
//{
//	if (_uniType == E_uniType::UNI_INT)
//	{
//		int* value = static_cast<int*>( _value);
//		setUniformi( _uniformName, *value);
//	}
//	else if (_uniType == E_uniType::UNI_FLOAT)
//	{
//		float* value = static_cast<float*>( _value);
//		setUniformf( _uniformName, *value);
//	}
//	else if (_uniType == E_uniType::UNI_VEC3)
//	{
//		glm::vec3* value = static_cast<glm::vec3*> (_value);
//		setUniform( _uniformName, *value);
//	}
//	else if (_uniType == E_uniType::UNI_MAT4)
//	{
//		glm::mat4* value = static_cast<glm::mat4*> (_value);
//		setUniform( _uniformName, *value);
//	}
//	else
//		error(std::string("Error: wrong uniform type for uniform: ") + _uniformName);
//		
//}


////////////////////////////////////////////////////////////////////////////////
// ADD NEW UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::addUniform(const std::string& _uniform)
{
	//m_uniforms[E_UNIFORM_TRANSFORM] = glGetUniformLocation(m_program, _uniform);
	int uniformLocation = glGetUniformLocation(m_data->program, _uniform.c_str());
	if (uniformLocation == -1)
	{
		error(std::string("Error: uniform ") + _uniform + std::string(" not found in program: ") + m_data->fileName);
		return;
	}
		
	if ( !m_uniforms.insert( std::make_pair(_uniform, uniformLocation)).second )
	{
		error(std::string("Error: uniform ") + _uniform + std::string(" already present in program:") + m_data->fileName);
	}

}


////////////////////////////////////////////////////////////////////////////////
// SET INT UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniformi( const std::string& _uniformName, int _value) const
{
	glUniform1i( m_uniforms[_uniformName], _value);
}


////////////////////////////////////////////////////////////////////////////////
// SET FLOAT UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniformf( const std::string& _uniformName, float _value) const
{
	glUniform1f( m_uniforms[_uniformName], _value);
}


////////////////////////////////////////////////////////////////////////////////
// SET GLM::VEC3 UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniform( const std::string& _uniformName, glm::vec3 _value) const
{
	glUniform3fv( m_uniforms[_uniformName],1, glm::value_ptr(_value));
}


////////////////////////////////////////////////////////////////////////////////
// SET GLM::MAT4 UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniform( const std::string& _uniformName, glm::mat4 _value) const
{
	glUniformMatrix4fv(m_uniforms[_uniformName], 1, GL_FALSE, glm::value_ptr(_value));
}


////////////////////////////////////////////////////////////////////////////////
// SET BASE LIGHT UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniform	(const std::string& _uniformName,	BaseLight _baseLight) const
{
	setUniform(_uniformName + ".color", _baseLight.color);
	setUniformf(_uniformName + ".intensity", _baseLight.intensity);
}


////////////////////////////////////////////////////////////////////////////////
// SET DIRECTIONAL LIGHT UNIFORM
////////////////////////////////////////////////////////////////////////////////
void IShader::setUniform	(const std::string& _uniformName,	DirectionalLight _dirLight) const
{
	setUniform(_uniformName + ".base", _dirLight.base);
	setUniform(_uniformName + ".direction", _dirLight.direction);
}


//PRIVATE:
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CREATE SHADER
////////////////////////////////////////////////////////////////////////////////
GLuint IShader::createShader(const std::string& _shaderSource, E_ShaderType _shaderType)
{
	debug("creating shader: " + m_data->fileName + "type: " + std::string((_shaderType == E_ST_Vertex) ? "vertex" : "fragment"));
	GLenum shaderType;
	switch (_shaderType)
	{
		case E_ST_Vertex:
			shaderType = GL_VERTEX_SHADER; 
			break;
		case E_ST_Fragment:
			shaderType = GL_FRAGMENT_SHADER; 
			break;
		default:
			error ( "wrong shader type");
			return -1;
	}


	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		error(std::string("Error: Shader creation failed"));

	const GLchar* shaderSource[1];
	GLint shaderSourceLengths[1];

	shaderSource[0] = _shaderSource.c_str();
	shaderSourceLengths[0] = _shaderSource.length();

	glShaderSource(shader, 1, shaderSource, shaderSourceLengths);
	glCompileShader(shader);

	this->checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Program compilation failed");

	return shader;
}


////////////////////////////////////////////////////////////////////////////////
// LOAD SHADER FROM FILE
////////////////////////////////////////////////////////////////////////////////
std::string IShader::loadShader ( const std::string& _fileName )
{
	std::ifstream file;
	std::string output;
	std::string line;
		
	file.open(_fileName.c_str());
	if ( file.is_open() )
	{
		while ( file.good() )
		{
			getline(file, line);
			output.append(line + '\n');
		}
	}
	else
		error(std::string("Unable to load shader: ") + _fileName);

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// BIND ATTRIBUTE LOCATION
////////////////////////////////////////////////////////////////////////////////
void IShader::bindAttribLocation(const GLchar* _attrib, unsigned int _location)
{
	glBindAttribLocation(m_data->program, _location, _attrib);
}


////////////////////////////////////////////////////////////////////////////////
// LINK PROGRAM
////////////////////////////////////////////////////////////////////////////////
void IShader::linkProgram(GLuint _program)
{
	glLinkProgram(m_data->program);
	checkShaderError(m_data->program, GL_LINK_STATUS, true, "Error: Program linking failed");

	glValidateProgram(m_data->program);
	checkShaderError(m_data->program, GL_VALIDATE_STATUS, true, "Error: Program is invalid");
}


////////////////////////////////////////////////////////////////////////////////
// CHECK SHADER ERRORS
////////////////////////////////////////////////////////////////////////////////
void IShader::checkShaderError(GLuint _shader, GLuint _flag, bool _isProgram, const std::string& _errorMessage)
{
	GLint success = 0;
	GLchar errorMsg[ERR_MSG_SIZE] = { 0 };

	if (_isProgram)
		glGetProgramiv(_shader, _flag, &success);
	else
		glGetShaderiv(_shader, _flag, &success);

	if ( success == GL_FALSE )
	{
		if ( _isProgram )
			glGetProgramInfoLog(_shader, sizeof(errorMsg), NULL, errorMsg);
		else
			glGetShaderInfoLog(	_shader, sizeof(errorMsg), NULL, errorMsg);

		error( std::string(_errorMessage) + std::string(": ") + std::string(errorMsg) );
	}
}

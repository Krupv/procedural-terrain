#include "Mesh.hpp"

#include "../core/Vertex.hpp"
#include "../static_libs/obj_loader.h"


// INDEXED MODEL
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
// CALCULATE NORMALS
////////////////////////////////////////////////////////////////////////////////
void IndexedModel::calcNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v1 = positions[i1] - positions[i0];
        glm::vec3 v2 = positions[i2] - positions[i0];

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for(unsigned int i = 0; i < positions.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}


// MESH 
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

// PUBLIC
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh( Vertex* _p_verticies, unsigned int _numVerticies, unsigned int* _indicies, unsigned int _numIndicies, bool _calcNormal )
{

	IndexedModel model;

	for ( unsigned int i = 0; i < _numVerticies ; i++ )
	{
		model.positions.push_back(*_p_verticies[i].getPos());
		model.texCoords.push_back(*_p_verticies[i].getTexCoords());
		model.normals.push_back(*_p_verticies[i].getNormal());
	}
	
	for ( unsigned int i = 0; i < _numIndicies ; i++ )
		model.indices.push_back(_indicies[i]);

	if (_calcNormal)
		calcNormal(&model);
		// model.CalcNormals();
	
	initMesh(model);

}

Mesh::Mesh(const std::string& fileName)
{
	initMesh(OBJModel(MESH_FOLDER + fileName).ToIndexedModel());
}


Mesh::Mesh(const IndexedModel& _model)
{
	// if (_calcNormal)
	// 	calcNormal(_model);
		// model.CalcNormals();
	
	initMesh(_model);

}
////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VAB[E_POSITION_VB]);
	glDeleteBuffers(1, &m_VAB[E_TEXCOOR_VB]);
	glDeleteBuffers(1, &m_VAB[E_NORMAL_VB]);
	glDeleteBuffers(1, &m_VAB[E_IBO_VB]);

	glDeleteVertexArrays(1, &m_VAO);

}


////////////////////////////////////////////////////////////////////////////////
// INICIALIZE MESH  FROM INDEXEDMODEL
////////////////////////////////////////////////////////////////////////////////
void Mesh::initMesh(const IndexedModel& _model)
{
	m_drawCount = _model.indices.size();	

	glError();
	glGenVertexArrays(1, &m_VAO);

	glError();
	glBindVertexArray(m_VAO);

	glError();
	glGenBuffers(E_NUM_BUFFERS, m_VAB);

	glError();
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[E_POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, _model.positions.size() * sizeof(_model.positions[0]), &_model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glError();

	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[E_TEXCOOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, _model.texCoords.size() * sizeof(_model.texCoords[0]), &_model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glError();
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[E_NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_model.normals[0]) * _model.normals.size(), &_model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VAB[E_IBO_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _model.indices.size() * sizeof(_model.indices[0]), &_model.indices[0], GL_STATIC_DRAW);

	glError();
	glBindVertexArray(0);
}


////////////////////////////////////////////////////////////////////////////////
// DRAW MESH
////////////////////////////////////////////////////////////////////////////////
void Mesh::draw() const
{
// glError();
	glBindVertexArray(m_VAO);
// glError();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VAB[E_IBO_VB]);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
// glError();
	glBindVertexArray(0);
}




// PRIVATE
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// CALCULATE NORMALS
////////////////////////////////////////////////////////////////////////////////
void Mesh::calcNormal(IndexedModel* _model)
{
	for ( unsigned int i = 0; i < _model->indices.size(); i +=3 )
	{
		glm::vec3 v0 = _model->positions[ _model->indices[i] ];
		glm::vec3 v1 = _model->positions[_model->indices[i + 1] ];
		glm::vec3 v2 = _model->positions[_model->indices[i + 2] ];
		
		glm::vec3 normal = glm::normalize( glm::cross( v1 - v0, v2 - v0) );

		_model->normals[ _model->indices[ i     ] ] += normal;
		_model->normals[ _model->indices[ i + 1 ] ] += normal;
		_model->normals[ _model->indices[ i + 2 ] ] += normal;

	}

	////const glm::vec3 up = worldUp; //static from Transform.hpp
	for ( unsigned int i = 0; i < _model->normals.size() ; i++ )
	{
		_model->normals[i] = glm::normalize( _model->normals[i] );
	//	
	//	//TODO up for slope picking, that will need color buffer
	}
}

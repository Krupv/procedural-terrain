#include "Transform.hpp"

#ifdef THREAD_SAFE
	#include <thread>
	#include <mutex>
#endif

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>



Transform::Transform(const glm::vec3& _position, const glm::quat& _rotation, const float& _scale, Transform* _parent) : 
		m_position(_position),
		m_rotation(_rotation),
		m_scale(_scale)
{
	if (!_parent)
	{
		m_transParent = nullptr;
		m_parentMatrix = glm::mat4(glm::mat4(1.f));
	}
	else
	{
		m_transParent = _parent;
		m_parentMatrix = m_transParent->getTransMat(); 
	}

#ifdef THREAD_SAFE
	m_transWrite = new std::mutex;
	m_absTransWrite = new std::mutex;
	m_parentTransWrite = new std::mutex;
#endif

	m_transChanged = true;
	m_transform = this->getTransMat();
	
	//weird hack, look at default position than rotate by some amount
	//needs inversed up for some reason, after transform inicialization up is normal
	// this->lookAt( glm::vec3 (0.f, 0.f, -1.f), glm::vec3(0.f,-1.f,0.f));
	//this->lookAt( glm::vec3 (0.f, 0.f, 1.f), glm::vec3(0.f,1.f,0.f));
	this->rotate(_rotation);
}

Transform::~Transform()
{ 
#ifdef THREAD_SAFE
	delete m_transWrite;
	delete m_absTransWrite;
	delete m_parentTransWrite;
#endif
}

//GET DIRECTION VECTORS
//--------------------------------------------------------------------------------
const glm::vec3		Transform::getRight()	const	
{ 
	return glm::normalize(glm::rotate(m_rotation, glm::vec3(1,0,0))); 
}

const glm::vec3		Transform::getUp()		const	
{ 
	return glm::normalize(glm::rotate(m_rotation, glm::vec3(0,1,0)));	
}

const glm::vec3		Transform::getForward()const	
{ 
	return glm::normalize(glm::rotate(m_rotation, glm::vec3(0,0,1))); 
}


//POSITION
//------------------------------------------------------------------------------
void Transform::setPos (const glm::vec3& _position)
{ 
	m_transChanged = true; 

	m_position = _position;	
}

const glm::vec3& Transform::getPos() const	
{ 
	return m_position;	
}

const glm::vec3 Transform::getAbsolutePos() const
{
	if (m_transParent)
		return m_transParent->getAbsolutePos() + m_position;
	else
		return m_position;
}

void	Transform::move	(const glm::vec3& _direction, const float& _amount)
{ 
	m_transChanged = true; 

	m_position += _direction * _amount;	
}

 //move with limit, for primitive collition with terrain
#define PRECISION 5.f
void	Transform::moveCollisY (const glm::vec3& _direction, const float& _amount, float _limitY)
{ 
	m_transChanged = true; 

	m_position += _direction * _amount;

	if ((m_position.y <= (_limitY + PRECISION )) )
	{
		m_position.y = _limitY + PRECISION;
	}

}

//END POSITION


//ROTATION
//------------------------------------------------------------------------------
const glm::quat& Transform::getRot()	const	
{ 
	return m_rotation;
}

glm::quat Transform::getAbsoluteRot() const
{
	if (m_transParent)
		return glm::normalize(m_transParent->getAbsoluteRot() * m_rotation);
	else
		return m_rotation;
}

void	Transform::setRot  (const glm::quat& _rotation)	
{ 
	m_transChanged = true;
	m_rotation = _rotation;	
}

void Transform::rotate	        (const glm::vec3& _axis, float _angle)
{
	m_transChanged = true;
	this->rotate(glm::normalize(glm::angleAxis(glm::radians(_angle), _axis)));
}

void Transform::rotate	        (const glm::quat& _rotation)
{
	m_transChanged = true;
	m_rotation = glm::normalize(glm::quat(  _rotation * m_rotation ));
}
//END ROTATION


//SCALE
//------------------------------------------------------------------------------
float		Transform::getScale()		const	
{ 
	return m_scale;
}

float Transform::getAbsoluteScale()		const
{
	if (m_transParent)
		return m_transParent->getAbsoluteScale() * m_scale ;
	else
		return m_scale;
}
void	Transform::setScale(const float& _scale)			
{ 
	m_transChanged = true;
	m_scale = _scale;
}
void	Transform::scale	(const float& _scale)			
{ 
	m_transChanged = true;
	m_scale += _scale;			
}
//END SCALE


//TRANSFORMATION MATRIX
//------------------------------------------------------------------------------
const glm::mat4& Transform::getTransMat()			const
{
	bool updatedParentMat = syncParentMatrix();
	bool updatedThisMat = syncTransMatrix();

	if ( updatedThisMat || updatedParentMat)
	{
		#ifdef THREAD_SAFE
			std::lock_guard<std::mutex> lock(*m_absTransWrite);
		#endif
		m_absoluteTransform = m_parentMatrix * m_transform;
	}

	return m_absoluteTransform;
}


void Transform::lookAt (const glm::vec3& _target = glm::vec3(0,0,-1), const glm::vec3& _up = worldUp)
{
	m_transChanged = true;
	glm::mat4 look = glm::lookAt(m_position, m_position + _target, _up);	
	m_rotation = glm::normalize(glm::toQuat(look));
}

bool Transform::syncTransMatrix() const
{

	if (m_transChanged)
	{
		#ifdef THREAD_SAFE
			std::lock_guard<std::mutex> lock(*m_transWrite);
		#endif

		m_transform = glm::translate(m_position) * glm::toMat4(m_rotation) * glm::scale( glm::vec3 (m_scale ,m_scale, m_scale));
		m_absoluteTransform = m_parentMatrix * m_transform;
		m_transChanged = false;

		return true; // indicates m_absoluteMatrix needs to be updated

	} // m_mutex unlock

	return false;  // indicates m_absoluteMatrix doesnt needs to be updated
}

bool Transform::syncParentMatrix() const
{
	if (m_transParent != nullptr && m_transParent->hasChanged() )
	{
		#ifdef THREAD_SAFE
			std::lock_guard<std::mutex> lock(*m_parentTransWrite);
		#endif
		m_parentMatrix = m_transParent->getTransMat();	

		return true;  // indicates m_absoluteMatrix needs to be updated
	}
	return false;  // indicates m_absoluteMatrix needs to be updated
}


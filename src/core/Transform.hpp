#pragma once
#include "../main.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#ifdef THREAD_SAFE
	namespace std {
		class mutex;
	}
	#include <memory>
#endif


static const glm::vec3 worldUp = glm::normalize(glm::vec3(0,1,0));

// Thread safe interface for ITransform
class ITransform {
public:
	virtual ~ITransform() {};

	//GET DIRECTION VECTORS
	virtual const glm::vec3		getRight()		const = 0;
	virtual const glm::vec3		getUp()			const = 0;
	virtual const glm::vec3		getForward()	const = 0;

	//POSITION
	virtual const glm::vec3&	getPos()			const = 0;
	virtual const glm::vec3		getAbsolutePos()	const = 0;

	//ROTATION
	virtual const glm::quat&	getRot()			const = 0;
	virtual glm::quat			getAbsoluteRot()	const = 0;

	//SCALE
	virtual float				getScale()			const = 0;
	virtual float				getAbsoluteScale()	const = 0;

	//TRANSFORMATION MATRIX
	virtual glm::mat4			getTransMat()	const = 0;
	virtual inline bool			hasChanged()	const = 0;
};

class Transform {
public:
	Transform(const glm::vec3& _position = glm::vec3(0,0,0), const glm::quat& _rotation = glm::quat(0,0,0,1), const float& _scale = 1.f, Transform* _parent = nullptr);
	~Transform();

	void setTransParent( Transform* _parent)
	{
		m_transParent = _parent;
	}

	//GET DIRECTION VECTORS
	const glm::vec3		getRight()	 const;	
	const glm::vec3		getUp()		 const;	
	const glm::vec3		getForward() const;	

	//POSITION
	void				setPos			(const glm::vec3& _position);	
	const glm::vec3&	getPos			() const;	
	const glm::vec3		getAbsolutePos	() const;
	void				move			(const glm::vec3& _direction, const float& _amount);	
	//DONE: maybye just use one direction and -amount for the other
	void				moveForward		(const float& _amount)		{ move(getForward(),_amount);	}
	void				moveRight		(const float& _amount)		{ move(getRight(), _amount);	}
	void				moveUp			(const float& _amount)		{ move(getRight(), _amount);	}
	//move with limit, for primitive collition with terrain
	void				moveCollisY			(const glm::vec3& _direction, const float& _amount,  float _limitY);
	void				moveForwardCollisY	(const float& _amount, float _limitY)		{ moveCollisY( getForward(), _amount, _limitY);	}
	void				moveRightCollisY	(const float& _amount, float _limitY)		{ moveCollisY( getRight(), _amount, _limitY);	}
	void				moveUpCollisY		(const float& _amount, float _limitY)		{ moveCollisY( getRight(), _amount, _limitY);	}

	//ROTATION
	const glm::quat&	getRot			()	const;
	glm::quat			getAbsoluteRot	()	const;
	void				setRot			(const glm::quat& _rotation);
	void				rotate			(const glm::vec3& _axis, float _angle);
	void				rotate			(const glm::quat& _rotation);

	//SCALE
	float				getScale		()		const;
	float				getAbsoluteScale()		const;
	void				setScale		(const float& _scale);
	void				scale			(const float& _scale);

	//TRANSFORMATION MATRIX
	const glm::mat4&			getTransMat		()		const;

	void				lookAt		(const glm::vec3& _target, const glm::vec3& _up);
	inline bool			hasChanged() const { return m_transChanged; };

protected:

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	float m_scale;

	mutable bool m_transChanged;
	mutable glm::mat4 m_transform;
	mutable glm::mat4 m_absoluteTransform;   // transform matrix togeter with parent matrix to
											// allow returning reference instead of copying the 'parent * transform Mat' every getTransMat call

	// glm::vec3 m_oldPosition;
	// glm::vec3 m_oldRotation;

#ifdef THREAD_SAFE
	std::mutex* m_transWrite;
	std::mutex* m_absTransWrite;
	std::mutex* m_parentTransWrite;
#endif

	Transform* m_transParent;
	mutable glm::mat4 m_parentMatrix;

	bool syncTransMatrix() const;
	bool syncParentMatrix() const;

};

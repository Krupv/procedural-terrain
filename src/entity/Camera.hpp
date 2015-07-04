#pragma once
#include "../main.hpp"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "IEntity.hpp"


static const glm::vec3 yAxis = glm::normalize(glm::vec3(0,1,0));

class Camera : public IEntity {
public:
	Camera( float _aspect, float _fov = 70.f, float _zNear = 0.1f, float _zFar = 10000.f);
	~Camera();

	glm::mat4 getViewProjection()	const;
	const glm::mat4& getProjection() const;
	glm::mat4 getView()	const;

	void setFOV		(float _fov);
	void setAspect	(float _aspect);

private:
	void update(float _delta);
	void redoProjection();

	glm::mat4 m_projection;
	float m_aspect;
	float m_fov;
	float m_zNear;
	float m_zFar;
};



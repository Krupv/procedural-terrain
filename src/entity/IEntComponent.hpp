#pragma once
#include "../main.hpp"

#include "IEntity.hpp"

class IEntComponent {
public:
	IEntComponent();

	virtual ~IEntComponent() {};

	void addOwner(IEntity* _owner);
	
	virtual void update(float _delta) {};

protected:
	IEntity* m_owner;
};

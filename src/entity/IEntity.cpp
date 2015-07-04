#include "IEntity.hpp"

#include "IEntComponent.hpp"


// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
IEntity::IEntity() : m_parent(nullptr) 
{ 

}

IEntity::IEntity(IEntity* _parent) 
{ 
	setParent( _parent); 
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
IEntity::~IEntity()
{

}



////////////////////////////////////////////////////////////////////////////////
// UPDATE ALL CHILDREN AND COMPONENTS
////////////////////////////////////////////////////////////////////////////////
//void updateAll(float _delta)
void IEntity::updateAll(TimePoint _delta)
{

	auto now = Clock::now();

	float delta = std::chrono::duration<float>(_delta - now).count();
	update(delta);

	for (unsigned int i = 0; i < m_components.size(); i++)
		m_components[i]->update(delta);

	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->updateAll(_delta);
}

////////////////////////////////////////////////////////////////////////////////
// RENDER ALL CHILDREN
////////////////////////////////////////////////////////////////////////////////
void IEntity::renderAll(const Tmatricies& _pipe) const
{
	render(_pipe);

	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->renderAll(_pipe);
}

////////////////////////////////////////////////////////////////////////////////
// ADD CHILD
////////////////////////////////////////////////////////////////////////////////
void IEntity::addChild(IEntity* _child)
{
	//Transform* tr = this;
	//std::shared_ptr<Transform> tt = std::make_shared<Transform>(*tr);
	_child->setParent(this);
	m_children.push_back(std::shared_ptr<IEntity>(_child));
}

////////////////////////////////////////////////////////////////////////////////
// ADD NEW COMPONENT
////////////////////////////////////////////////////////////////////////////////
void IEntity::addComponent(IEntComponent* _component)
{
	_component->addOwner(this);
	m_components.push_back(std::unique_ptr<IEntComponent>(_component));
}


IEntity* m_parent;

// PRIVATE
//------------------------------------------------------------------------------
void IEntity::setParent(IEntity* _parent)
{
	setTransParent( _parent);
	m_parent = _parent;

}

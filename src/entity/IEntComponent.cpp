#include "IEntComponent.hpp"


// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
IEntComponent::IEntComponent()
{

}

////////////////////////////////////////////////////////////////////////////////
// ADD OWNER OF THIS COMPONENT
////////////////////////////////////////////////////////////////////////////////
void IEntComponent::addOwner(IEntity* _owner)
{
	m_owner = _owner;

}



// PRIVATE
//------------------------------------------------------------------------------



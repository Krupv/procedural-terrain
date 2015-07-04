#pragma once
#include "../main.hpp"
#include "../core/Transform.hpp"
#include <vector>


#include "../rendering/IShader.hpp" //just for now for Tmatricies

class IEntComponent;

//TMP
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;


class IEntity : public Transform {
public:
	IEntity();
	IEntity(IEntity* _parent);

	~IEntity();

	//void updateAll(float _delta)
	void updateAll(TimePoint _delta);
	void renderAll(const Tmatricies& _pipe) const;
	void addChild(IEntity* _child);
	void addComponent(IEntComponent* _component);


protected:
	IEntity* m_parent;

	virtual void render(const Tmatricies& _pipe) const {};
	virtual void update(float _delta) {};

private:

	std::vector <std::shared_ptr<IEntity>> m_children;
	std::vector <std::unique_ptr<IEntComponent>> m_components;
	

	//only sets parent without adding to its children
	//TODO: either add to children too or make it protected
	void setParent(IEntity* _parent);
};



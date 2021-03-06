#ifndef entity_h__
#define entity_h__

#include "engine/component/component.h"

class EntityPrototype;
class Entity;

namespace entitysys
{
	void destroyEntity( Entity* ent );
}

class Entity 
{
	friend class EntityPrototype;
	friend void entitysys::destroyEntity( Entity* ent );

public:
	Entity(void);
	~Entity(void);

	void addComponent( Component* c );
	void removeComponent( Component* c );

	template <class T>
	T* getComponent();

	template <class T>
	bool hasComponent();

	EntID ID;

protected:
	
	std::list<Component*> mComponents;
};



template <class T>
T* Entity::getComponent()
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if( typeid(**i)== typeid(T) )
		{
			return static_cast<T*>(*i);
		}
	}
	assert( "Component doesn't exist - check it exists first" );
	return nullptr;
}
template <class T>
bool Entity::hasComponent()
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if( typeid(**i) == typeid(T) )
		{
			return true;
		}
	}
	return false;
}

#endif // entity_h__

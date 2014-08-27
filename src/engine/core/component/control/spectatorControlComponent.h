#pragma once

#include "engine/core/event/EventListener.h"
#include "engine/core/component/base/component.h"
#include "engine/core/component/physics/physicsComponent.h"

// No easy way around this one, it needs to be passed a physics component.
class SpectatorControlComponent : public Component, public EventListener
{
public:
	SpectatorControlComponent( PhysicsComponent* physicsComponent );
	~SpectatorControlComponent(void);

	void handle( Event* e );
	void update(  double dt );

private:
	PhysicsComponent* mPhysicsComponent;

	bool mForward;
};


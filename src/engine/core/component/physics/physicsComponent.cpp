#include "pch.h"
#include "physicsComponent.h"
#include "engine/core/component/base/entity.h"

PhysicsComponent::PhysicsComponent( PhysicsManager* physicsManager )
{
	mPhysicsManager = physicsManager;
}


PhysicsComponent::~PhysicsComponent(void)
{
}

void PhysicsComponent::setAsBox( float x, float y, float z )
{
	btCollisionShape* boxShape = new  btBoxShape( btVector3( x,y,z ));
	initialise(boxShape, 1, float3(0,30,0) );
}

void PhysicsComponent::update(  double dt  )
{
	//TODO Updates entity data here
	//mParent->mPosition = Ogre::Vector3( mBody->getWorldTransform().getOrigin().getX(),mBody->getWorldTransform().getOrigin().getY(),mBody->getWorldTransform().getOrigin().getZ() );

	MovementEvent* e = new MovementEvent(EV_Movement);
	e->mPosition = mBody->getWorldTransform().getOrigin();
	e->mOrientation = mBody->getWorldTransform().getRotation();

	dispatch( e );
}

void PhysicsComponent::initialise( btCollisionShape* shape, btScalar mass, float3 position, Quat orientation )
{
	btVector3 nullVector(0,0,0);
	shape->calculateLocalInertia( 1, nullVector);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass,
		new btDefaultMotionState( btTransform( orientation, position))
		, shape, nullVector);

	mBody = new btRigidBody(fallRigidBodyCI);
	mPhysicsManager->getDiscreteDynamicsWorld()->addRigidBody(mBody);
}
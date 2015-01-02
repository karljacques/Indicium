//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "pch.h"
#include "engine.h"
#include "manager\network\clientNetworkSystem.h"
#include "manager\network\serverNetworkSystem.h"
#include "manager\abstract\spectatorManager.h"

Engine::Engine()
{
	SDL_Init( SDL_INIT_EVERYTHING );

    m_EngineShutdown = false;

    // Set the working directory
    char* basePath = SDL_GetBasePath();
#ifdef WIN32
	SetCurrentDirectoryA(basePath);
#else
	chdir(basePath);
#endif

	// Create Systems
    new EventSystem();
    mEntityManager = new EntityManager();

	mPhysicsManager = new PhysicsManager();
	mNetworkSystem = new ServerNetworkSystem( );
	
	mRenderSystem = new RenderSystem( mEntityManager );
	mInputSystem = new InputSystem(  mRenderSystem->getSDLWindow() );
	mCameraManager = new CameraManager( mRenderSystem );

	mSpectatorManager = new SpectatorManager( mEntityManager );

	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );
	EventSystem::getSingletonPtr()->registerListener( this );

	// Register all the listening managers as listeners
	EventSystem* e = EventSystem::getSingletonPtr();
	e->registerListener(mPhysicsManager);
	e->registerListener(mRenderSystem);
	e->registerListener(mCameraManager);
	e->registerListener(mSpectatorManager);

	{
		// Create a spectator
		Entity* ent = mEntityManager->createEntity();

		CameraComponent* comp = mCameraManager->createComponent(ent->LUID);
		mCameraManager->createNewCamera( comp );
		ent->addComponent(comp);

		SpectatorComponent* spec = mSpectatorManager->createComponent(ent->LUID);
		ent->addComponent(spec);

		PhysicsComponent* phys = mPhysicsManager->createComponent(ent->LUID);
		mPhysicsManager->initComponent(phys,new btBoxShape( btVector3(1,1,1) ), 1, float3(40,0,0), Quat(0,0,0,1) );
		phys->body->setGravity(float3(0,0,0)); // Disable gravity on a spectator
		ent->addComponent(phys);
		phys->body->setDamping( 0.9f, 1.0f );

		mCameraManager->lookAt( comp, float3( 0,0,0 ));
	}
	
	{

		// Create a static ground
		Entity* box = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(box->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,1,100));
		box->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(box->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 0.5f,50.0f ) ), 0,float3(0,0,0), Quat(0,0,0,1));
		box->addComponent(phys);

	}

	/* Create console - Singleton
		Registers itself as an event listener */
	new OgreConsole(this);

	OgreConsole::getSingleton().addCommand( "net.connect", &Console_Net_Connect );
	OgreConsole::getSingleton().addCommand( "net.status", &Console_Net_Status );

	OgreConsole::getSingleton().addCommand( "geo.spawn", &Console_Geometry_Spawn );
	
}

Engine::~Engine()
{
	SDL_Quit();

    delete mInputSystem;
	delete mRenderSystem;
}

void Engine::update()
{
    // Make sure to pump messages in all render windows
    Ogre::WindowEventUtilities::messagePump();

	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMillisecondsCPU();
	OgreConsole::getSingletonPtr()->print(std::to_string(dt));
	mTimeSinceLastUpdate.reset();

	// Update systems and managers
    mInputSystem->update();

	// profile event system
	EventSystem::getSingletonPtr()->handleEvents();

	mPhysicsManager->update( dt );
	mSpectatorManager->update();

	mNetworkSystem->receive();

    // render after everything is updated
    mRenderSystem->renderOneFrame();
}

bool Engine::isShuttingDown()
{
    return m_EngineShutdown;
}

void Engine::handle( Event* e )
{
    switch( e->getEventType() )
    {
        case EV_CORE_KEY_PRESS:
			{
				KeyboardEvent* ke = e->getData<KeyboardEvent>();
				switch (ke->mKeycode)
				{
					case SDL_SCANCODE_ESCAPE: // Escape
					   m_EngineShutdown = true;
					break;

				}
			}
        break;

    }
}


void Engine::SetAsClient(const char* ip)
{
	EventSystem::getSingletonPtr()->deregisterListener(mNetworkSystem);
	delete mNetworkSystem;

	this->mNetworkSystem = new ClientNetworkSystem( );
	EventSystem::getSingletonPtr()->registerListener(this->mNetworkSystem);
	static_cast<ClientNetworkSystem*>(mNetworkSystem)->connect( ip );

	
}

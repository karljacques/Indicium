#include "pch.h"
#include "playerManager.h"
#include "../../core/user-interface/ogreConsole.h"
#include "../../core/event/events/playerEvent.h"

PlayerManager::PlayerManager(NetworkSystem* networkSystem) : mNetworkSystem(networkSystem)
{
	// Get my GUID
	mLocalPlayer= RakNet::RakNetGUID::ToUint32( mNetworkSystem->getPeer()->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS) );

	// Add local player to the list
	shared_ptr<Player> local = shared_ptr<Player>( new Player() );
	local->GUID = mLocalPlayer;
	this->addPlayer( local );
}

void PlayerManager::addPlayer(EntID guid, std::string username)
{
	// Construct player
	shared_ptr<Player> player( new Player() );
	player->GUID = guid;
	player->username = username;

	// Copy the shared ptr to the map
	mConnectedPlayers[guid] = player;
}

void PlayerManager::addPlayer(shared_ptr<Player> player)
{
	// If object constructed elsewhere
	mConnectedPlayers[player->GUID] = player;
}

bool PlayerManager::playerExists(EntID guid)
{
	return ( mConnectedPlayers.count(guid) > 0 );
}

shared_ptr<Player> PlayerManager::getPlayerByGUID(EntID guid)
{
	if( mConnectedPlayers.count(guid) > 0 )
		return mConnectedPlayers[guid];
	else
	{
		assert( "Should be checking that player exists!!");
		return nullptr;
	}
}

void PlayerManager::removePlayerByGUID(EntID guid)
{
	mConnectedPlayers.erase( guid );
}

void PlayerManager::constructFromData(char* data)
{

}

void PlayerManager::handle(Event* e)
{
	switch( e->getEventType() )
	{
	case EV_NETWORK_PLAYER_DATA:
		{
			// Add new player - this will only be called when I am host
			PlayerEvent* p = e->getData<PlayerEvent>();
			this->addPlayer( p->GUID, std::string( p->username ) );

			OgreConsole::getSingletonPtr()->print("New Player Connecting: " + std::string( p->username ) );

			// As I am host, inform clients of new player
			// Clone event, send to all under 'EV_NETWORK_PLAYER_JOINED'
			Event* ne = new Event( EV_NETWORK_PLAYER_JOINED );
#ifdef _DEBUG
			ne->d_initialised = true;
#endif
			ne->clone( e );
			this->mNetworkSystem->send( ne, HIGH_PRIORITY, RELIABLE );
			delete ne;

			break;
		}
	case EV_NETWORK_PLAYER_JOINED:
		{
			OgreConsole::getSingletonPtr()->print("EV_NETWORK_PLAYER_JOINED");

			PlayerEvent* p = e->getData<PlayerEvent>();
			this->addPlayer(p->GUID, std::string( p->username ) );

			OgreConsole::getSingletonPtr()->print( "New player has joined: " + std::string( p->username ) );

			break;
		}

	case EV_NETWORK_NEW_CONNECTION:
		{
			// Signal from the network system that we've connected to a host, we'll need to send it our player data
			Player* p = this->getPlayerByGUID(mLocalPlayer).get();
			Event* ne = new Event( EV_NETWORK_PLAYER_DATA );
			PlayerEvent* pe = ne->createEventData<PlayerEvent>();
#ifdef _DEBUG
			ne->d_initialised = true;
			ne->GUID = 1337;
#endif
			assert( p->username.size() < USERNAME_LENGTH );

			memcpy( pe->username, p->username.c_str(), p->username.size() + 1 );

			mNetworkSystem->send(ne, HIGH_PRIORITY,RELIABLE );
			delete ne;
			break;
		}
	}
}

void PlayerManager::update(double dt)
{

}

shared_ptr<Player> PlayerManager::getLocalPlayer()
{
	return mConnectedPlayers[mLocalPlayer];
}



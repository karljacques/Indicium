#ifndef playerEvent_h__
#define playerEvent_h__

#define USERNAME_LENGTH 20

struct PlayerEvent
{
	PlayerEvent()
	{
		pGUID = 0;
		for( int i=0; i < USERNAME_LENGTH; i++ )
		{
			username[i] = 0;
		}
	}

	char username[USERNAME_LENGTH];
	PlayerGUID pGUID;
};
#endif // playerEvent_h__

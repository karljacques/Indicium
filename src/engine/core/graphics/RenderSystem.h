//
//  gfxSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__gfxSystem__
#define __YorkshireTea__gfxSystem__

#include "camera.h"
#include <SDL.h>
class Camera;
    
class RenderSystem
{
public:
        
    RenderSystem();
    ~RenderSystem();
        
    // Getters
    Ogre::Root*           getOgreRoot();
    Ogre::SceneManager*   getSceneMgr();
    Ogre::RenderWindow*   getRenderWindow();
    Ogre::SceneNode*      getRootSceneNode();
	SDL_Window*			  getSDLWindow(){ return mWindow; }
        
    std::vector<Camera*>*    getCameraList();
        
    // Functions
    void            renderOneFrame();
	Ogre::String	generateName(const Ogre::String& prefix = "Unnamed" );
   
private:
        
    // Core
    Ogre::Root*           m_Root;
    Ogre::SceneManager*   m_SceneMgr;
    Ogre::RenderWindow*   m_RenderWindow;
    Ogre::SceneNode*      m_RootSceneNode;

    // Multiple Viewport and Camera support
    std::vector<Camera*>    m_CameraList;
        
    // Window
    size_t          m_WindowHandle;
	SDL_Window*			  mWindow;
};

#endif /* defined(__YorkshireTea__gfxSystem__) */

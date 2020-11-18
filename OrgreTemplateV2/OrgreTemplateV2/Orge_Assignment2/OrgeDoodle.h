#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreTrays.h"
#include "OgreRTShaderSystem.h"
#include "AudioEngine.h"
#include <iostream>
#include <list>

using namespace Ogre;
using namespace OgreBites;

class Doodle;
class Platform;
class Background;

// The Orge app
class OgreDoodle : public ApplicationContext, public InputListener
{
	// Orge Objects
	RenderWindow* mWindow;
	Root* mRoot;
	SceneManager* mSceneMan;
	TrayManager* mTrayMan;

	Label* ScoreLabel;
	Label* FPSLabel;
	Label* TPULabel;

	SceneNode* camNode;

	Background* mBackground;
	Doodle* mDoodle;
	std::list<Platform*> mPlatforms;

	// Audio Engine
	DoofAudioEngine Audio;

	// Game and Window Data
	int iScore = 0;
	int iWindowHeight = 800;
	int iWindowWidth = 600;

	float displayDelay = 1.0f;

public:
	//Construtor and Destructor
	OgreDoodle();
	virtual ~OgreDoodle();

	void setup();
	void run();

private:
	// Overloaded App Event Functions
	bool keyPressed(const KeyboardEvent& evt)
	{
		return processEvents(evt);
	}
	bool frameStarted(const FrameEvent& frameEvent)
	{
		ApplicationContext::frameStarted(frameEvent);
		return update(frameEvent);
	}
	bool frameEnded(const FrameEvent& frameEvent)
	{
		ApplicationContext::frameEnded(frameEvent);
		return render(frameEvent);
	}

	// OrgePong Event Functions
	bool processEvents(const KeyboardEvent& evt);
	bool update(const FrameEvent& frameEvent);
	bool render(const FrameEvent& frameEvent);
};

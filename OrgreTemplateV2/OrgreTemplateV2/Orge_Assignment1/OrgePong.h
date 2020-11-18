#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreTrays.h"
#include "OgreRTShaderSystem.h"
#include <iostream>

using namespace Ogre;
using namespace OgreBites;

class Ball; 
class Paddle;

// The Orge app
class OgrePong : public ApplicationContext, public InputListener
{
	// Orge Objects
	RenderWindow* mWindow;
	Root* mRoot;
	SceneManager* mSceneMan;
	TrayManager* mTrayMan;

	Label* LivesLabel;
	Label* ScoreLabel;
	
	Ball* mBall;
	Paddle* mPaddle;

	// Game and Window Data
	int iLives = 5;
	int iScore = 0;
	int iWindowHeight = 800;
	int iWindowWidth = 600;

public:
	//Construtor and Destructor
	OgrePong();
	virtual ~OgrePong();

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
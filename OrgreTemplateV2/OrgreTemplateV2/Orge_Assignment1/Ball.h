#pragma once

#include "OrgePong.h"

using namespace Ogre;
using namespace OgreBites;


class Ball
{
	SceneNode* mNode;
	Entity* mEntity;
	float xVelocity;
	float yVelocity;

public:

	Ball(SceneManager* scnMan);
	~Ball();

	// Getters and Setters
	Vector3 GetPosition() { return mNode->getPosition(); }
	Entity* GetShape() { return mEntity; }

	// Game Functions
	void hitBottom();
	void reboundBatTop();
	void reboundSides();
	void update(const Ogre::FrameEvent& evt);
};

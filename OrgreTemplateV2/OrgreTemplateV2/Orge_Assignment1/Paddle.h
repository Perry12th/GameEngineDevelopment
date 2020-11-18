#pragma once

#include "OrgePong.h"

using namespace Ogre;
using namespace OgreBites;

enum Movement
{
	IDLE,
	GOING_LEFT,
	GOING_RIGHT
};

class Paddle
{
	SceneNode* mNode;
	Entity* mEntity;
	float speed;
	int leftSideLimit;
	int rightSideLimit;

public:
	Paddle(SceneManager* scnMan, int windowWidth);
	~Paddle();

	Movement direction = IDLE;

	Vector3 GetPosition() { return mNode->getPosition(); }
	Entity* GetShape() { return mEntity; }

	void update(const Ogre::FrameEvent& evt);
};

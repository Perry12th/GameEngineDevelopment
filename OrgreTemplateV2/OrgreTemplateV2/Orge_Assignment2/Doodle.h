#pragma once

#include "OrgeDoodle.h"

using namespace Ogre;
using namespace OgreBites;

class Doodle
{
	SceneNode* mNode;
	Entity* mEntity;
	float yAccel;
	float yVelocity;
	float xVelocity;

public:

	Doodle(SceneManager* scnMan);
	~Doodle();

	// Getters and Setters
	Vector3 GetPosition() { return mNode->getPosition(); }
	Entity* GetEntity() const { return mEntity; }
	float GetVelocity() { return yVelocity; }
	Entity* GetShape() { return mEntity; }

	// Game Functions
	void GoLeft();
	void GoRight();
	void Jump();
	void update(const FrameEvent& evt);
};

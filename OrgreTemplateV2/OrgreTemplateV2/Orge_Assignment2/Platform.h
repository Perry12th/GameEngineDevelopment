#pragma once
#include "OrgeDoodle.h"

using namespace Ogre;
using namespace OgreBites;

class Platform
{
	SceneNode* mNode;
	Entity* mEntity;

public: 
	Platform(SceneManager* scnMan, MaterialPtr Material);
	~Platform();

	// Getters and Setters
	Vector3 GetPosition() const { return mNode->getPosition(); }
	void SetPosition(const Vector3& position) { mNode->translate(position); }
	Entity* GetShape() { return mEntity; }
};

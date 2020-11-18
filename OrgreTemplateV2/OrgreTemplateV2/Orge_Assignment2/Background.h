#pragma once
#include "OrgeDoodle.h"

using namespace Ogre;
using namespace OgreBites;

class Background
{
	SceneNode* mNode;
	Entity* mEntity;

public:
	Background(SceneManager* scnMan);
	~Background();
	 
	// Getters and Setters
	Vector3 GetPosition() const { return mNode->getPosition(); } 
	void SetPosition(const Vector3& position) { mNode->translate(position); }
};

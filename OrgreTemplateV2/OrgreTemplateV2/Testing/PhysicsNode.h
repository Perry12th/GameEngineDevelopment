#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include <iostream>

using namespace Ogre;
using namespace OgreBites;
using namespace std;

static float Gravity = -9.8f;

class PhysicsNode
{
	SceneNode* PhysicNode;
	Vector3 Velocity;
	float Mass = 5.0f;
	bool GravityEnabled = true;

public:


	PhysicsNode()
	{

	}

	void UpdateNode(const FrameEvent& evt)
	{
		if (GravityEnabled)
		{
			Velocity.y = (Velocity.y > Gravity * Mass * 2 ? Velocity.y -= (Mass * evt.timeSinceLastFrame * -Gravity) : Gravity * Mass * 2);
		}
		PhysicNode->translate(Velocity * evt.timeSinceLastFrame);
	}

	void setGravityEnabled(bool gravityEnabled)
	{
		GravityEnabled = gravityEnabled;
	}

	bool getGravityEnabled()
	{
		return GravityEnabled;
	}

	void setPhysicsNode(SceneNode* physicNode)
	{
		PhysicNode = physicNode;
	}

	void setVelocity(Vector3 velocity)
	{
		Velocity = velocity;
	}

	Vector3 getVelocity()
	{
		return Velocity;
	}

	void setMass(float newMass)
	{
		Mass = newMass;
	}

	float getMass()
	{
		return Mass;
	}

	SceneNode* getPhysicsNode()
	{
		return PhysicNode;
	}
};

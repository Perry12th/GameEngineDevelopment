#pragma once
#include "DoodleGuy.h"

class DoodleGamePhysicsEngine
{

public:
	bool gameOver = false;
	DoodleGuy* doodleGuy;
	std::list<Platform*> platforms;
	std::list<PhysicsNode*> physicsNodes;

	DoodleGamePhysicsEngine()
	{

	}

	void AddPlatform(Platform* newPlatform)
	{
		platforms.push_back(newPlatform);
	}

	void SetPlayerPhysicsNode(DoodleGuy* doodleGuy)
	{
		this->doodleGuy = doodleGuy;
	}

	void AddPhysicsNode(PhysicsNode* physicsNode)
	{
		physicsNodes.push_back(physicsNode);
	}

	float getGravity()
	{
		return Gravity;
	}

	bool getGameOver()
	{
		return gameOver;
	}

	void UpdatePhysicsNodes(const FrameEvent& evt)
	{
		doodleGuy->getPhysicNode()->UpdateNode(evt);
		doodleGuy->updateCamera(evt);
		cout << "Running" << endl;

		if (doodleGuy->getJumpTimer() != nullptr)
		{
			if (doodleGuy->getJumpTimer()->getMilliseconds() > 5000)
			{
				// Game Over
				cout << "Game Over!" << endl;
				system("Pause");
				exit(1);
			}
		}
		for(PhysicsNode* physicNode : physicsNodes)
		{
			physicNode->UpdateNode(evt);

			if (doodleGuy->getPhysicNode()->getVelocity().y < 0.0f)
			{
				if (doodleGuy->getPhysicNode()->getPhysicsNode()->_getWorldAABB().intersects(physicNode->getPhysicsNode()->_getWorldAABB()))
				{
					doodleGuy->Jump();
				}

			}
			else
			{
				for (Platform* platform : platforms)
				{
					if (platform->getPlatformNode()->getPosition().distance(doodleGuy->getPhysicNode()->getPhysicsNode()->getPosition()) > 80.0f)
					{
						platform->randomSpawnLocation(doodleGuy->getPhysicNode()->getPhysicsNode()->getPosition());
					}
				}
			}
		}
	}
};
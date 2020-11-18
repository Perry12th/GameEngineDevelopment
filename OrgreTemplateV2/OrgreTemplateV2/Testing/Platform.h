#pragma once
#include "PhysicsNode.h"

class Platform
{
	Vector3 spawnLocation;
	SceneNode* PlatformNode;

public:
	Platform(SceneManager* scnMgr, int platformNumber)
	{
        SceneNode* platformNode;

        ManualObject* platformMan = scnMgr->createManualObject("PlatformMan " + platformNumber);

        platformMan->setDynamic(false);

        platformMan->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);

        platformMan->position(-5.f, -1.f, 0.f);
        platformMan->position(5.f, -1.f, 0.f);
        platformMan->position(5.f, 1.f, 0.f);
        platformMan->position(-5.f, 1.f, 0.f);

        platformMan->triangle(0, 1, 2);
        platformMan->triangle(0, 2, 3);

        platformMan->end();
        
        platformNode = scnMgr->getRootSceneNode()->createChildSceneNode("PlatformMan " + platformNumber);

        PlatformNode = platformNode;

        PlatformNode->attachObject(platformMan);
        PlatformNode->setScale(Vector3(2.f, 1.f, 0.f));

        spawnLocation = randomSpawnLocation(Vector3(0.0f));

        PlatformNode->setPosition(spawnLocation);
	}

    Vector3 randomSpawnLocation(Vector3 baseLocation)
    {
        Vector3 newLocation(Ogre::Math::RangeRandom(-80.0f, 60.0f), Ogre::Math::RangeRandom(-50.0f, 60.0f) + baseLocation.y, 0.0f);
        PlatformNode->setPosition(newLocation);

        return newLocation;
    }

    SceneNode* getPlatformNode()
    {
        return PlatformNode;
    }
};
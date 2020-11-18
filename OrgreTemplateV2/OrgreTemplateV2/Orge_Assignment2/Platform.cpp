#include "Platform.h"

Platform::Platform(SceneManager* scnMan, MaterialPtr material)
{
	mEntity = scnMan->createEntity(SceneManager::PrefabType::PT_PLANE);
	mNode = scnMan->getRootSceneNode()->createChildSceneNode();
	// Set material and position
	mEntity->setMaterial(material);
	mNode->attachObject(mEntity);
	mNode->setScale(0.5, 0.15, 1);
	mNode->setPosition(Vector3(0, -100, 0));
}

Platform::~Platform()
{
}

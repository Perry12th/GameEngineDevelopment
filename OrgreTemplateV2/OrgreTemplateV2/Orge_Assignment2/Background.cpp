#include "Background.h"

Background::Background(SceneManager* scnMan)
{
	mEntity = scnMan->createEntity(SceneManager::PrefabType::PT_PLANE);
	mNode = scnMan->getRootSceneNode()->createChildSceneNode();
	// Create Player Material
	MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("background.png");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	// Set material and position
	mEntity->setMaterial(material);
	mNode->attachObject(mEntity);
	mNode->setScale(5.0f, 5.0f, 1.0f);
	mNode->setPosition(Vector3(0, 0, -50));
}

Background::~Background()
{
}

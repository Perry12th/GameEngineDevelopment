#include "Doodle.h"

Doodle::Doodle(SceneManager* scnMan)
{
	mEntity = scnMan->createEntity(SceneManager::PrefabType::PT_PLANE);
	mNode = scnMan->getRootSceneNode()->createChildSceneNode();
	// Create Player Material
	MaterialPtr material = MaterialManager::getSingleton().create("Player", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("doodle.png");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	// Set material and position
	mEntity->setMaterial(material);
	mNode->attachObject(mEntity);
	mNode->setScale(0.3, 0.3, 1);
	mNode->setPosition(Vector3(0, 0, 0));
}

Doodle::~Doodle()
{
}

void Doodle::GoLeft()
{
	xVelocity = -250;
}

void Doodle::GoRight()
{
	xVelocity = 250;
}

void Doodle::Jump()
{
	yAccel = 550;
	yVelocity = 0;
}

void Doodle::update(const FrameEvent& evt)
{
	// Update the accel (giving it gravity)
	yAccel += -10.0f;

	// Update the yVelocity (clamp the value)
	yVelocity = Math::Clamp(yVelocity += yAccel, -200.0f, 500.0f);

	// Update the position
	mNode->translate(Vector3(xVelocity, yVelocity, 0) * evt.timeSinceLastFrame);
}

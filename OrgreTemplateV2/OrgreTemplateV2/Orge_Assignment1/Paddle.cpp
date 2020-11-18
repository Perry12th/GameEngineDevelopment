#include "Paddle.h"

Paddle::Paddle(SceneManager* scnMan, int windowWidth)
{
	mEntity = scnMan->createEntity("cube.mesh");
	mNode = scnMan->getRootSceneNode()->createChildSceneNode();
	mNode->setPosition(Vector3(0, -300, 0));
	mNode->setScale(3.0, 0.5, 1.0);
	mNode->attachObject(mEntity);
	speed = 500.0f;
	leftSideLimit = -windowWidth + 60;
	rightSideLimit = windowWidth - 60;

}

Paddle::~Paddle()
{
}

void Paddle::update(const Ogre::FrameEvent& evt)
{
	if (direction == GOING_LEFT)
	{
		mNode->translate(Vector3(-speed, 0, 0) * evt.timeSinceLastFrame);

		// Checkking bounds
		if (mNode->getPosition().x < leftSideLimit)
		{
			mNode->setPosition(Vector3(leftSideLimit, mNode->getPosition().y, mNode->getPosition().z));
		}
	}

	else if (direction == GOING_RIGHT)
	{
		mNode->translate(Vector3(speed, 0, 0) * evt.timeSinceLastFrame);

		// Checking bounds
		if (mNode->getPosition().x > rightSideLimit)
		{
			mNode->setPosition(Vector3(rightSideLimit, mNode->getPosition().y, mNode->getPosition().z));
		}
	}

	direction = IDLE;
}

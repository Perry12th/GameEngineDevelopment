#include "Ball.h"

Ball::Ball(SceneManager* scnMan)
{
	mEntity = scnMan->createEntity("ogrehead.mesh");
	mNode = scnMan->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mEntity);
	mNode->setPosition(Vector3(0,0,0));

	xVelocity = Math::RangeRandom(-40, 40);
	yVelocity = -200;
}
Ball::~Ball()
{
}

void Ball::hitBottom()
{
	// Reset the position of the ball
	mNode->setPosition(Vector3(0, 0, 0));

	xVelocity = Math::RangeRandom(-40, 40);
	yVelocity = -200;
}

void Ball::reboundBatTop()
{
	yVelocity = -yVelocity * 1.01;
}

void Ball::reboundSides()
{
	xVelocity = -xVelocity + 1.05;
}

void Ball::update(const Ogre::FrameEvent& evt)
{
	mNode->translate(Vector3(xVelocity, yVelocity, 0) * evt.timeSinceLastFrame);
}

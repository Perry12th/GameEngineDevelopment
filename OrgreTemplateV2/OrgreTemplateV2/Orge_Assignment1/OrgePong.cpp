#include "OrgePong.h"
#include "Ball.h"
#include "Paddle.h"

OgrePong::OgrePong() : ApplicationContext("OrgePong")
{
}

OgrePong::~OgrePong()
{
}

void OgrePong::setup()
{
	// first call the base function
	ApplicationContext::setup();
	addInputListener(this);

	// get a pointer to the root 
	mRoot = getRoot();
	mSceneMan = mRoot->createSceneManager();

	// register the game(scene) woth the RTSS
	RTShader::ShaderGenerator* shaderGen = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGen->addSceneManager(mSceneMan);

	// Set AmbientLight
	mSceneMan->setAmbientLight(ColourValue(0.8, 0.2, 0.8));

	// Set the sceneLight
	Light* light = mSceneMan->createLight("MainLight");
	SceneNode* lightNode = mSceneMan->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	// Place the light behind the camera
	lightNode->setPosition(-80, 80, 50);

	//Speaking of Camera
	SceneNode* camNode = mSceneMan->getRootSceneNode()->createChildSceneNode();

	// Setting up the Camera
	Camera* cam = mSceneMan->createCamera("myCam");
	cam->setProjectionType(PT_ORTHOGRAPHIC); // 2D
	cam->setNearClipDistance(5); // For this example
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setPosition(0, 0, 140);

	// Render the game(scene) to the main window
	mWindow = getRenderWindow();
	mWindow->addViewport(cam);
	mWindow->resize(iWindowWidth, iWindowHeight);

	// Set up the ball
	mBall = new Ball(mSceneMan);

	// Set up the paddle
	mPaddle = new Paddle(mSceneMan, iWindowWidth);

	// Set up the hud
	mTrayMan = new TrayManager("HUD", getRenderWindow());
	mSceneMan->addRenderQueueListener(getOverlaySystem());

	LivesLabel = mTrayMan->createLabel(TL_TOPLEFT, "Lives", "Lives: " + std::to_string(iLives), 150);
	ScoreLabel = mTrayMan->createLabel(TL_TOPLEFT, "Score", "Score: " + std::to_string(iScore), 150);
	mTrayMan->showFrameStats(TL_TOPRIGHT);

}

void OgrePong::run()
{
	mRoot->startRendering();
}

bool OgrePong::processEvents(const KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case SDLK_ESCAPE:
		getRoot()->queueEndRendering();
		break;
	case SDLK_LEFT:
		mPaddle->direction = GOING_LEFT;
		break;
	case SDLK_RIGHT:
		mPaddle->direction = GOING_RIGHT;
		break;
	default:
		break;
	}
		
	return true;
}

bool OgrePong::update(const FrameEvent& frameEvent)
{
	// Preform the movement
	mBall->update(frameEvent);
	mPaddle->update(frameEvent);

	// Check collisions
	if (mBall->GetShape()->getWorldBoundingBox(true).intersects(mPaddle->GetShape()->getWorldBoundingBox(true)))
	{
		mBall->reboundBatTop();
		iScore += 5;
		ScoreLabel->setCaption("Score: " + std::to_string(iScore));
	}
	else if (mBall->GetPosition().x <= -iWindowWidth || mBall->GetPosition().x >= iWindowWidth)
	{
		mBall->reboundSides();
	}

	else if (mBall->GetPosition().y >= (iWindowHeight / 2 + 100))
	{
		mBall->reboundBatTop();
	}

	else if (mBall->GetPosition().y <= (-iWindowHeight / 2 -100))
	{
		iLives--;
		LivesLabel->setCaption("Lives: " + std::to_string(iLives));
		if (iLives == 0)
		{
			getRoot()->queueEndRendering(); // Quit the game
		}
		else
		{
			mBall->hitBottom();
		}
	}
	// Update the hud

	return true;
}

bool OgrePong::render(const FrameEvent& frameEvent)
{
	return true;
}

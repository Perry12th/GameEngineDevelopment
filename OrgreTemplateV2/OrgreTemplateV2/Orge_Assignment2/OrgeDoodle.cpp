#include "OrgeDoodle.h"
#include "Doodle.h"
#include "Background.h"
#include "Platform.h"

OgreDoodle::OgreDoodle() : ApplicationContext("OrgeDoodle")
{
}

OgreDoodle::~OgreDoodle()
{
	Audio.Shutdown();
}

void OgreDoodle::setup()
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
	camNode = mSceneMan->getRootSceneNode()->createChildSceneNode();

	// Setting up the Camera
	Camera* cam = mSceneMan->createCamera("myCam");
	cam->setProjectionType(PT_ORTHOGRAPHIC); // 2D
	cam->setNearClipDistance(5); // For this example
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setPosition(0, 0, 10);

	// Render the game(scene) to the main window
	mWindow = getRenderWindow();
	mWindow->addViewport(cam);
	mWindow->resize(iWindowWidth, iWindowHeight);

	// Set up the background
	//mBackground = new Background(mSceneMan);
	// Set up doodle
	mDoodle = new Doodle(mSceneMan);
	// set up the platforms
	// Create Player Material
	MaterialPtr material = MaterialManager::getSingleton().create("Platform", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("platform.png");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	for (int i = 0; i < 10; i++)
	{
		Vector3 platformPosition = Vector3(Math::RangeRandom(-150.0f, 150.0f), -200.0f + (i * 150.0f), 0.0f);
		auto platform = new Platform(mSceneMan, material);
		platform->SetPosition(platformPosition);
		mPlatforms.push_back(platform);
	}


	// Set up the hud
	mTrayMan = new TrayManager("HUD", getRenderWindow());
	mSceneMan->addRenderQueueListener(getOverlaySystem());

	ScoreLabel = mTrayMan->createLabel(TL_BOTTOM, "Score", "Score: " + std::to_string(iScore), 150);
	FPSLabel = mTrayMan->createLabel(TL_TOPLEFT, "FPS", "FPS:", 150);
	TPULabel = mTrayMan->createLabel(TL_TOPRIGHT, "Time/Update", "Time/Update:", 150);
	// Play some music
	Audio.Init();

	Audio.LoadSound("Audio\jump.mp3");
FILE* file;

	file = fopen("Audio\jump.mp3", "r");

	if (!file)
	{
		std::cout << "File not found" << endl;
	}

}

void OgreDoodle::run()
{
	mRoot->startRendering();
}

bool OgreDoodle::processEvents(const KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case SDLK_ESCAPE:
		getRoot()->queueEndRendering();
		break;
	case SDLK_LEFT:
		mDoodle->GoLeft();
		break;
	case SDLK_RIGHT:
		mDoodle->GoRight();
		break;
	case SDLK_SPACE:
		mDoodle->Jump();
		break;
	default:
		break;

	}
	return true;

}

bool OgreDoodle::update(const FrameEvent& frameEvent)
{
	Audio.Update();

	displayDelay -= frameEvent.timeSinceLastFrame;

	if (displayDelay < 0.0)
	{
		displayDelay = 1.0f;
		// Calculate and show FPS
		FPSLabel->setCaption("FPS: " + std::to_string((int)(1 / frameEvent.timeSinceLastFrame)));

		// Calculate and show TPU
		TPULabel->setCaption("Time/Update " + std::to_string(frameEvent.timeSinceLastFrame));
	}
	// Preform the movement
	mDoodle->update(frameEvent);
	// Check collisions and positions for platforms
	for (Platform* platform : mPlatforms)
	{

		if (platform->GetShape()->getWorldBoundingBox(true).intersects(mDoodle->GetShape()->getWorldBoundingBox(true))
			&& platform->GetPosition().y < mDoodle->GetPosition().y
			&& mDoodle->GetVelocity() <= 0)
		{
			//std::cout << "Platform hit";
			Audio.PlaySounds("Audio\jump.mp3", mDoodle->GetPosition(), 60.0f);
			mDoodle->Jump();
		}

		if (platform->GetPosition().y < iScore - 500)
		{
			Vector3 platformPosition = Vector3(Math::RangeRandom(-150.0f, 150.0f), 1300.0f, 0.0f);
			platform->SetPosition(platformPosition);
		}
	}

	if (mDoodle->GetPosition().y < iScore - 500)
	{
		mDoodle->Jump();
	}
	else if (mDoodle->GetPosition().y > iScore)
	{
		iScore = mDoodle->GetPosition().y;
		ScoreLabel->setCaption("Score: " + std::to_string(iScore));
		camNode->setPosition(Vector3(camNode->getPosition().x, iScore, camNode->getPosition().z));
	}
	return true;
}

bool OgreDoodle::render(const FrameEvent& frameEvent)
{
	return true;
}

//! [fullsource]

//#include "Ogre.h"
//#include "OgreApplicationContext.h"
//#include "OgreInput.h"
//#include "OgreRTShaderSystem.h"
//#include <iostream>
#include "DoodleGamePhysicsEngine.h"
#include "OgreTrays.h"

using namespace Ogre;
using namespace OgreBites;
using namespace std;

class PhysicsUpdateListener : public FrameListener
{
    DoodleGamePhysicsEngine physicsEngine;

    Label* FPS;
    Label* DeltaTime;

    Timer timer;
    int timemil = 500;
    int timeScale = 1;

public:
    PhysicsUpdateListener(DoodleGamePhysicsEngine physicsEngine, Label* FPS, Label* DeltaTime)
    {
        this->physicsEngine = physicsEngine;
        this->FPS = FPS;
        this->DeltaTime = DeltaTime;

        timer.reset();
    }

    bool frameStarted(const FrameEvent& evt)
    {
        Real fps = 1 / evt.timeSinceLastFrame;
        Real deltatime = evt.timeSinceLastFrame;

        if (timer.getMilliseconds() > (timemil * timeScale))
        {
            timeScale++;
            FPS->setCaption("FPS: " + Ogre::DisplayString(to_string(fps)));
            DeltaTime->setCaption("DeltaTime: " + Ogre::DisplayString(to_string(deltatime)));
        }

        physicsEngine.UpdatePhysicsNodes(evt);

        return true;
    }
};

class Assignment2
    : public ApplicationContext
    , public InputListener
{
public:

    DoodleGamePhysicsEngine physicsEngine;
    DoodleGuy* doodleGuy;

    TrayListener myTrayListener;
    Label* FPS;
    Label* DeltaTime;
    TrayManager* mTrayMgr;

    DisplayString LivesString = "Lives: 3";
    DisplayString ScoreString = "Score: 0";

    Assignment2();
    virtual ~Assignment2() {}

    void setup();
    bool keyPressed(const KeyboardEvent& evt);
    void createFrameListener();
};


Assignment2::Assignment2()
    : ApplicationContext("Assignment2_101193046_Matthew_Pereira")
{
}


void Assignment2::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    
    addInputListener(this);
    
    // get a pointer to the already created root
    Root* root = getRoot();
    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- tutorial section start --
    //! [turnlights]
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    //! [turnlights]

    //! [newlight]
    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    //! [newlight]

    //! [lightpos]
    lightNode->setPosition(20, 80, 50);
    //! [lightpos]

    //! [camera]
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Camera* cam = scnMgr->createCamera("myCam");
    //cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    camNode->setPosition(0, 0, 150);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);
    //! [camera]

    TrayManager* mTrayMgr = new TrayManager("InterfaceName", getRenderWindow());

    scnMgr->addRenderQueueListener(mOverlaySystem);

    addInputListener(mTrayMgr);

    FPS = mTrayMgr->createLabel(TL_BOTTOMRIGHT, "FPS", "", 200);
    DeltaTime = mTrayMgr->createLabel(TL_BOTTOMLEFT, "DeltaTime", "", 200);

    // set our doodleguy pointer to a new object
    doodleGuy = new DoodleGuy(scnMgr, camNode);

    // Set our character in our physics Engine
    physicsEngine.SetPlayerPhysicsNode(doodleGuy);

    // ~~~~~~~~~~~~~~~~~~~

    for (int i = 0; i < 10; i++)
    {
        Platform* platform;
        platform = new Platform(scnMgr, i + 1);

        PhysicsNode* platformPhysicsNode;
        platformPhysicsNode = new PhysicsNode();

        platformPhysicsNode->setPhysicsNode(platform->getPlatformNode());
        platformPhysicsNode->setGravityEnabled(false);
        physicsEngine.AddPhysicsNode(platformPhysicsNode);
        physicsEngine.AddPlatform(platform);
    }

    SceneNode* platformNode;

    ManualObject* platformMan = scnMgr->createManualObject("PlatformMan");

    platformMan->setDynamic(false);

    platformMan->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);

    platformMan->position(-5.f, -1.f, 0.f);
    platformMan->position(5.f, -1.f, 0.f);
    platformMan->position(5.f, 1.f, 0.f);
    platformMan->position(-5.f, 1.f, 0.f);

    platformMan->triangle(0, 1, 2);
    platformMan->triangle(0, 2, 3);

    platformMan->end();

    platformNode = scnMgr->getRootSceneNode()->createChildSceneNode("PlatformMan");

    PhysicsNode* backgrounplatformPhysicsNode;
    backgrounplatformPhysicsNode = new PhysicsNode();

    backgrounplatformPhysicsNode->setPhysicsNode(platformNode);

    backgrounplatformPhysicsNode->getPhysicsNode()->attachObject(platformMan);
    backgrounplatformPhysicsNode->getPhysicsNode()->setScale(Vector3(30.f, 5.f, 0.f));//testNode->attachObject(PaddleMan);
    backgrounplatformPhysicsNode->getPhysicsNode()->setPosition(Vector3(-15.0f, -55.0f, 0.0f));//testNode->setScale(Vector3(30.f, 5.f, 0.f));
    //platformPhysicsNode->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
    backgrounplatformPhysicsNode->setGravityEnabled(false);

    physicsEngine.AddPhysicsNode(backgrounplatformPhysicsNode);

    // ~~~~~~~~~~~~~~~~~~~

    createFrameListener();

}

bool Assignment2::keyPressed(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    //case SDLK_SPACE:
    //    doodleGuy->Jump();
    //    break;

    case SDLK_LEFT:
        doodleGuy->MoveLeft();
        break;

    case SDLK_RIGHT:
        doodleGuy->MoveRight();
        break;

    default:
        break;
    }
    return true;
}

void Assignment2::createFrameListener()
{
    FrameListener* FrameListener = new PhysicsUpdateListener(physicsEngine, FPS, DeltaTime);
    mRoot->addFrameListener(FrameListener);
}

int main(int argc, char** argv)
{
    try
    {
        Assignment2 app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [fullsource]

#pragma comment(lib, "winmm.lib")
#include "Platform.h"
#include <Windows.h>
#include <mmsystem.h>

class DoodleGuy
{
	SceneNode* cameraNode;
	PhysicsNode* DoodleGuyPhysicsNode;
	Entity* doodleEntity;
	Timer* jumpTimer;
	float speed = 20.0f;

public:
	DoodleGuy(SceneManager* scnMgr, SceneNode* camNode)
	{
		cameraNode = camNode;
		SceneNode* doodleGuyNode;

		ManualObject* DoodleGuyMan = scnMgr->createManualObject("DoodleGuyMan");

		doodleEntity = scnMgr->createEntity(SceneManager::PrefabType::PT_PLANE);

		MaterialPtr doodleManMaterial = MaterialManager::getSingleton().create("DoodleManMat", "General");
		doodleManMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("doodle.png");
		doodleManMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		doodleManMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		doodleManMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		
		doodleEntity->setMaterial(doodleManMaterial);
		
		//DoodleGuyMan->setMaterial(1, doodleManMaterial);

		DoodleGuyMan->setDynamic(false);

		DoodleGuyMan->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);

		DoodleGuyMan->position(0.f, 0.f, 0.f);
		DoodleGuyMan->position(1.f, 0.f, 0.f);
		DoodleGuyMan->position(1.f, 1.f, 0.f);
		DoodleGuyMan->position(0.f, 1.f, 0.f);

		DoodleGuyMan->triangle(0, 1, 2);
		DoodleGuyMan->triangle(0, 2, 3);

		DoodleGuyMan->end();

		doodleGuyNode = scnMgr->getRootSceneNode()->createChildSceneNode("DoodleGuyMan");

		doodleGuyNode->attachObject(doodleEntity);
		
		PhysicsNode* doodleGuyPhysicsNode;
		doodleGuyPhysicsNode = new PhysicsNode();

		doodleGuyPhysicsNode->setPhysicsNode(doodleGuyNode);

		doodleGuyPhysicsNode->getPhysicsNode()->attachObject(DoodleGuyMan);
		doodleGuyPhysicsNode->getPhysicsNode()->setScale(Vector3(0.1f, 0.1f, 0.f));
		doodleGuyPhysicsNode->getPhysicsNode()->setPosition(Vector3(-15.0f, -50.0f, 0.0f));
		doodleGuyPhysicsNode->setVelocity(Vector3(0.0f, 10.0f, 0.0f));
		doodleGuyPhysicsNode->setGravityEnabled(true);

		setPhysicsNode(doodleGuyPhysicsNode);
	}

	void setPhysicsNode(PhysicsNode* physicsNode)
	{
		DoodleGuyPhysicsNode = physicsNode;
	}

	PhysicsNode* getPhysicNode()
	{
		return DoodleGuyPhysicsNode;
	}

	void updateCamera(const FrameEvent& evt)
	{
		cameraNode->setPosition(Vector3(0.0f, DoodleGuyPhysicsNode->getPhysicsNode()->getPosition().y + 20.0f, 140.0f));
	}

	void setSpeed(float newSpeed)
	{
		speed = newSpeed;
	}

	float getSpeed()
	{
		return speed;
	}

	Timer* getJumpTimer()
	{
		return jumpTimer;
	}

	void Jump()
	{
		PlaySound(TEXT("Jump.wav"), NULL, SND_ASYNC);
		jumpTimer = new Timer();
		DoodleGuyPhysicsNode->setVelocity(Vector3(DoodleGuyPhysicsNode->getVelocity().x,
			DoodleGuyPhysicsNode->getMass() * speed,
			DoodleGuyPhysicsNode->getVelocity().z));
	}

	void MoveLeft()
	{
		DoodleGuyPhysicsNode->setVelocity(Vector3(-speed,
			DoodleGuyPhysicsNode->getVelocity().y,
			DoodleGuyPhysicsNode->getVelocity().z));
	}

	void MoveRight()
	{
		DoodleGuyPhysicsNode->setVelocity(Vector3(speed,
			DoodleGuyPhysicsNode->getVelocity().y,
			DoodleGuyPhysicsNode->getVelocity().z));
	}
};


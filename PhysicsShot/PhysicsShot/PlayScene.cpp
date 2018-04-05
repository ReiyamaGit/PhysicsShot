/*
*�t�@�C�����FPlayScene.cpp
*��  ��  �ҁFhatakeyama
*��  ��  ���F2017/06/22
*�T      �v�FPlayScene�N���X�̒�`
*/

#include "PlayScene.h"
#include "Camera.h"
#include "Quad.h"
#include "Fbx.h"
#include "Light.h"
#include "InputInterface.h"
#include "PullDown.h"
#include "TouchListener.h"
#include "Label.h"
#include "PhysicsBody.h"



/************************************************************
 *	@brief			�R���X�g���N�^
 ************************************************************/
PlayScene::PlayScene()
{

}

/************************************************************
 *	@brief			�f�X�g���N�^
 ************************************************************/
PlayScene::~PlayScene()
{

}



PlayScene* PlayScene::Create()
{
	PlayScene* ret = new PlayScene();

	if (ret && ret->Init())
	{
		ret->AutoRelease();
		delete ret;
	}


	R_SAFE_DELETE(ret);

	return nullptr;

}




PlayScene* PlayScene::CreateWithPhysicsWorld()
{
	PlayScene* ret = new PlayScene();
	PhysicsWorld* physicsWorld = new PhysicsWorld();

	if (!ret)
	{
		return nullptr;
	}

	ret->m_physicsWorld = physicsWorld;

	if (physicsWorld && physicsWorld->Init() && ret->Init())
	{
		ret->AutoRelease();

		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;

}

/************************************************************
 *	@brief			����������
 ************************************************************/
bool PlayScene::Init()
{

	if (!DebugScene::Init())
	{
		return false;
	}

	{

		Fbx* torus = Fbx::Create("Assets/FBX/TestCube.fbx");
		
		torus->m_transform->SetParent(this->m_transform);
		PhysicsBody::CreateBox(m_physicsWorld, torus,1.0f);
		m_torus = torus;


	}

	{

		Fbx* torus = Fbx::Create("Assets/FBX/TestCube.fbx");
		torus->m_transform->SetParent(this->m_transform);
		PhysicsBody* body = PhysicsBody::CreateBox(m_physicsWorld, torus);
	}

	Input();

	return true;
}




/************************************************************
 *	@brief			�X�V����������
 ************************************************************/
bool PlayScene::Update(float delta)
{
	if (!DebugScene::Update(delta))
	{
		return false;
	}

	return true;
}




/************************************************************
 *	@brief			���͏���
 ************************************************************/
void PlayScene::Input()
{

}


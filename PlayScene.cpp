/*
*ファイル名：PlayScene.cpp
*作  成  者：hatakeyama
*作  成  日：2017/06/22
*概      要：PlaySceneクラスの定義
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
 *	@brief			コンストラクタ
 ************************************************************/
PlayScene::PlayScene()
{

}

/************************************************************
 *	@brief			デストラクタ
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
 *	@brief			初期化する
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
		torus->m_transform->m_position = D3DXVECTOR3(0.6, 5,0);
		PhysicsBody::CreateBox(m_physicsWorld, torus,1.0f);
		m_torus = torus;


	}

	{

		Fbx* torus = Fbx::Create("Assets/FBX/Torus.fbx");
		torus->m_transform->SetParent(this->m_transform);
		PhysicsBody* body = PhysicsBody::CreateSphere(m_physicsWorld, torus,0.f,1.5f);
	}

	Input();

	return true;
}




/************************************************************
 *	@brief			更新処理をする
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
 *	@brief			入力処理
 ************************************************************/
void PlayScene::Input()
{

}


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


	Fbx* sCube = Fbx::Create("Assets/FBX/TestCube.fbx");
	sCube->m_transform->SetParent(this->m_transform);
	sCube->m_transform->m_position = D3DXVECTOR3(1.5, 1, -3);
	PhysicsBody::CreateBox(m_physicsWorld, sCube, 1.0f);

	Fbx* sCube2 = Fbx::Create("Assets/FBX/TestCube.fbx");
	sCube2->m_transform->SetParent(this->m_transform);
	sCube2->m_transform->m_position = D3DXVECTOR3(1.5, 0, -3);
	PhysicsBody::CreateBox(m_physicsWorld, sCube2);

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			Fbx* dcube = Fbx::Create("Assets/FBX/TestCube.fbx");
			dcube->m_transform->SetParent(this->m_transform);
			dcube->m_transform->m_position = D3DXVECTOR3(x, y + 1, 0);
			PhysicsBody::CreateBox(m_physicsWorld, dcube, 1.0f);
		}
	}



	for (int x = 0; x < 5; x++)
	{
		Fbx* staticFbx = Fbx::Create("Assets/FBX/TestCube.fbx");
		staticFbx->m_transform->SetParent(this->m_transform);
		staticFbx->m_transform->m_position = D3DXVECTOR3(x, 0, 0);
		PhysicsBody::CreateBox(m_physicsWorld, staticFbx);
	}





	/*PullDown* test = PullDown::Create(Sprite::Create("Assets/UI/Creation.png"));
	test->AddList(Sprite::Create("Assets/UI/Translate.png"));
	test->AddList(Sprite::Create("Assets/UI/select.png"));
	test->AddList(Sprite::Create("Assets/UI/PlayMode.png"));
	test->RemoveAtIndex(1);
	test->m_transform->SetParent(this->m_transform);*/

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
	TouchListener* listener = TouchListener::Create();

	listener->onTouchBegan = [this](TouchListener::Touch touch)
	{
		SelectObjectFromScreen();

		if (m_selectedItem)
		{
			Fbx* item = dynamic_cast<Fbx*>(m_selectedItem);

			if (item)
			{
				item->GetPhysicsBody()->GetRigidBody()->applyCentralImpulse(btVector3(0, 0, 20));
			}
		}

		return true;
	};

	GLOBAL::Global::m_eventDispatcher->AddEventListener(listener);

}


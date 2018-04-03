/******************************************************
 *	@file		Scene.cpp
 *	@brief		Scene	実装
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/



//preproccesor--------------------------------
#include "Scene.h"
#include "Fbx.h"
#include "Camera.h"
#include "Light.h"
#include "Scheduler.h"
#include "Draw3DNode.h"
#include "ToggleButton.h"
#include "TouchListener.h"

#include <algorithm>




/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Scene::Scene()
:main_camera_(nullptr)
,m_physicsWorld(nullptr)
{
	
	m_referenceCount = 0;
}



/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Scene::~Scene()
{
	R_SAFE_DELETE(main_camera_);
	R_SAFE_DELETE(m_physicsWorld);
	
}





/******************************************************
*	@brief		初期化処理
*	@return		true: 成功	false:失敗
*	@remarks	子ノードの初期化処理をするため最初に
*				NodeクラスのInit()を呼んでいる
******************************************************/
bool Scene::Init()
{
	if (!Node::Init())
	{
		return false;
	}


	CountRetain();


	Camera* camera = Camera::Create();
	camera->m_transform->SetParent(this->m_transform);
	camera->m_transform->m_position = D3DXVECTOR3(0, 5, -10);
	camera->Update();
	main_camera_ = camera;
	GLOBAL::Global::m_mainCamera = camera;




	Light* light = Light::Create();
	light->m_transform->SetParent(this->m_transform);





	if (!camera)
	{
		R_ASSERT(camera);
	}


	if (!light)
	{
		R_ASSERT(light);
	}



	return true;
}



/****************************************************************************
 *	@brief		更新処理
 *	@return		PlaySceneのUpdateを呼ぶか
 true → 呼ぶ		false → 呼ばない

 *	@remarks	編集モードの場合はすべてのUpdateをスルーさせる
 *				最初にNodeのUpdateを呼ぶ	→	子ノードUpdate処理
 *				FPSを表示する
 *				描画順ソート
 *				isEdit	true ：PlaySceneのUpdateを呼ばない
 *						false：PlaySceneのUpdateを呼ぶ
 ****************************************************************************/
bool Scene::Update(float delta)
{
	if (!Node::Update(delta))
	{
		return false;
	}


	DrawSort(delta);



	return true;
}




/**********************************************
 *	@brief		描画順ソート
 *	@param		前フレームからの更新時間
 **********************************************/
void Scene::DrawSort(float delta)
{



	for (INT i = 0; i < m_transform->m_children.size(); i++)
	{
		m_transform->m_children[i]->m_attachNode->Update(delta);

		//---ソートするノードは距離を測る
		if (m_transform->m_children[i]->m_attachNode->GetIsSort())
		{
			//---カメラとノードの距離
			D3DXVECTOR3 v = GetCamera()->m_transform->m_position - m_transform->m_children[i]->m_position;
			m_transform->m_children[i]->m_attachNode->SetDistance(D3DXVec3Length(&v));
		}
		else
		{
			//---ソートしなくて一番前に持ってきたいやつ
			m_transform->m_children[i]->m_attachNode->SetDistance(999999);
		}
	}

	//---カメラからの距離が遠い順にソート
	std::sort(m_transform->m_children.begin(), m_transform->m_children.end(), NodeSort());
}



// Scene* Scene::CreateWithPhysicsWorld()
//{
//	 Scene* ret = new Scene();
//	 
//	 if (ret && ret->Init())
//	 {
//		 ret->AutoRelease();
//		 delete ret;
//	 }
//
//
//	 R_SAFE_DELETE(ret);
//
//	 return nullptr;
//
//}




 /*Scene* Scene::Create()
 {
	 Scene* ret = new Scene();
	 PhysicsWorld* physicsWorld = new PhysicsWorld();

	 if (ret && ret->Init() && physicsWorld && physicsWorld->Init())
	 {
		 ret->AutoRelease();
		 ret->m_physicsWorld = physicsWorld;
		 return ret;
	 }

	 R_SAFE_DELETE(ret);

	 return nullptr;

 }*/
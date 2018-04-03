/*********************************************
 *	@file		PhysicsWorld.cpp
 *	@brief		物理世界 実装
 *	@date		2018/03/21
 *	@name		hatakeyama
 ********************************************/


//preprocessor-------------------------------
#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include "Fbx.h"
#include "MyDebugDraw.h"


/**********************************************
 *	@brief		コンストラクタ
 **********************************************/
PhysicsWorld::PhysicsWorld()
	:m_isActive(true)
	, m_debuger(nullptr)
{
	m_collisionShapes.clear();
	m_physicsBodys.clear();
}





/**********************************************
 *	@brief		デストラクタ
 **********************************************/
PhysicsWorld::~PhysicsWorld()
{
	m_physicsBodys.shrink_to_fit();
	for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		m_collisionShapes[j] = 0;
		delete shape;
	}

	R_SAFE_DELETE(m_dynamicsWorld);
	R_SAFE_DELETE(m_collisionConfiguration);
	R_SAFE_DELETE(m_dispatcher);
	R_SAFE_DELETE(m_overlappingPairCache);
	R_SAFE_DELETE(m_solver);
	R_SAFE_DELETE(m_debuger);
	
}




/************************************************************************************
 *	@brief			初期化処理
 *	@return			true: 初期化成功	false:初期化失敗
 *	@remarks		1.衝突検出方法の選択 → Defaultに設定
 *					2.衝突判定監視の設定
 *					3.ブロードフェーズの設定
 *					4.拘束のソルバ設定
 *					5.物理世界設定
 ************************************************************************************/
bool PhysicsWorld::Init()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_overlappingPairCache = new btDbvtBroadphase();

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	if (!m_dynamicsWorld)
	{
		return false;
	}


	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	m_debuger = new MyDebugDraw();
	m_debuger->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	m_dynamicsWorld->setDebugDrawer(m_debuger);



	return true;
}





/***********************************************
 *	@brief			物理世界の更新・外力の計算
 *	@param[dt]		最終フレーム実行時間
 *	@remarks		重力の計算,そして位置の移動
 **********************************************/
bool PhysicsWorld::PhysicsUpdate(float dt)
{
	if (!m_isActive)
	{
		return false;
	}

	D3DXMATRIX rotate;
	D3DXQUATERNION q;
	btTransform trans;

	m_dynamicsWorld->stepSimulation(dt, 10);


	//
	for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{

		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		
		
		//-----World行列 取得-----
		trans = obj->getWorldTransform();
		

		//-----演算結果を取得・適用-----
		if (m_physicsBodys[j]->GetTarget())
		{
			
			m_physicsBodys[j]->GetTarget()->m_transform->m_position = D3DXVECTOR3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

			q = D3DXQUATERNION(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW());
			
			D3DXMatrixRotationQuaternion(&rotate, &q);
			
			m_physicsBodys[j]->GetTarget()->m_transform->m_btRotateMat = rotate;
		}

	}

	return true;
}





/***********************************************************************************
 *	@brief		DebugModeの操作で必要になる剛体の更新
 *	@remarks	PhysicsUpdate → 計算を中心にObjectが移動
 *				DebugPhysicsUpdate → クライアントの位置を中心にObjectが移動
 ***********************************************************************************/
void PhysicsWorld::DebugPhysicsUpdate()
{

	for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];


		btRigidBody* body = btRigidBody::upcast(obj);

		btTransform trans;

		trans = body->getWorldTransform();
		
		

		trans.setOrigin(btVector3(m_physicsBodys[j]->GetTarget()->m_transform->m_position.x
								, m_physicsBodys[j]->GetTarget()->m_transform->m_position.y
								, m_physicsBodys[j]->GetTarget()->m_transform->m_position.z));
		
		body->setWorldTransform(trans);

	}

}





/***********************************************************************************
 *	@brief		剛体情報が正しく設定されているかの確認
 ***********************************************************************************/
void PhysicsWorld::CheckPhysicsBody()
{
	for (int i = 0; i < m_physicsBodys.size(); i++)
	{
		if (m_physicsBodys[i]->GetTarget() == nullptr)
		{
			OutputDebugString("FBXにセットされていないPhysicsBodyがあります\n");
		}
	}
}
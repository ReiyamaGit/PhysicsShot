/******************************************************
*	@file		physicsWorld.h
*	@brief		physicsWorld 定義
*	@data		2017/11/28
*	@name		hatakeyama
******************************************************/

#ifndef __PHYSICSWORLD__
#define __PHYSICSWORLD__


//preprocessor----------------------------------------
#include "Global.h"
#include "btBulletDynamicsCommon.h"
#include <vector>


//prototype--------------------------------------------
class Fbx;
class PhysicsBody;
class MyDebugDraw;



class PhysicsWorld
{
private:
	bool m_isActive;

	MyDebugDraw* m_debuger;

	btDiscreteDynamicsWorld* m_dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	std::vector<PhysicsBody*> m_physicsBodys;

	btDefaultCollisionConfiguration* m_collisionConfiguration;

	btCollisionDispatcher* m_dispatcher;

	btBroadphaseInterface* m_overlappingPairCache;

	btSequentialImpulseConstraintSolver* m_solver;


public:

	PhysicsWorld();

	~PhysicsWorld();


	/************************************************************************************
	 *	@brief			初期化処理
	 *	@return			true: 初期化成功	false:初期化失敗
	 ************************************************************************************/
	bool Init();


	/**********************************************************************************************
	 *	@brief			物理世界の更新・外力の計算
	 *	@param[dt]		最終フレーム実行時間
	 ********************************************************************************************/
	bool PhysicsUpdate(float dt);



	/***********************************************************************************
	 *	@brief		DebugModeの操作で必要になる剛体の更新
	 ***********************************************************************************/
	void DebugPhysicsUpdate();
	
	


	/***********************************************************************************
	 *	@brief		剛体情報が正しく設定されているかの確認
	 ***********************************************************************************/
	void CheckPhysicsBody();


	/********************************************************************************
	 *	@brief		物理世界に剛体の追加
	 *******************************************************************************/
	inline void AddPhysicsBody(PhysicsBody* physicsBody) { m_physicsBodys.push_back(physicsBody); }


	/********************************************************************************
	 *	@brief		物理世界の適用ON・OFF
	 *******************************************************************************/
	inline void SetActive(bool isActive) { m_isActive = isActive; }



	/********************************************************************************
	 *	@brief		物理世界に剛体の追加
	 *******************************************************************************/
	inline bool GetActive() { return m_isActive; }



	/******************************************************************************
	 *	@brief		物理世界インスタンス取得
	 ******************************************************************************/
	inline btDynamicsWorld* GetDynamicsWorld() { return m_dynamicsWorld; }


	friend class PhysicsBody;
		
};

#endif __PHYSICSWORLD__
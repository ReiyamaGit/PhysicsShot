/******************************************************
*	@file		physicsWorld.h
*	@brief		physicsWorld ��`
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
	 *	@brief			����������
	 *	@return			true: ����������	false:���������s
	 ************************************************************************************/
	bool Init();


	/**********************************************************************************************
	 *	@brief			�������E�̍X�V�E�O�͂̌v�Z
	 *	@param[dt]		�ŏI�t���[�����s����
	 ********************************************************************************************/
	bool PhysicsUpdate(float dt);



	/***********************************************************************************
	 *	@brief		DebugMode�̑���ŕK�v�ɂȂ鍄�̂̍X�V
	 ***********************************************************************************/
	void DebugPhysicsUpdate();
	
	


	/***********************************************************************************
	 *	@brief		���̏�񂪐������ݒ肳��Ă��邩�̊m�F
	 ***********************************************************************************/
	void CheckPhysicsBody();


	/********************************************************************************
	 *	@brief		�������E�ɍ��̂̒ǉ�
	 *******************************************************************************/
	inline void AddPhysicsBody(PhysicsBody* physicsBody) { m_physicsBodys.push_back(physicsBody); }


	/********************************************************************************
	 *	@brief		�������E�̓K�pON�EOFF
	 *******************************************************************************/
	inline void SetActive(bool isActive) { m_isActive = isActive; }



	/********************************************************************************
	 *	@brief		�������E�ɍ��̂̒ǉ�
	 *******************************************************************************/
	inline bool GetActive() { return m_isActive; }



	/******************************************************************************
	 *	@brief		�������E�C���X�^���X�擾
	 ******************************************************************************/
	inline btDynamicsWorld* GetDynamicsWorld() { return m_dynamicsWorld; }


	friend class PhysicsBody;
		
};

#endif __PHYSICSWORLD__
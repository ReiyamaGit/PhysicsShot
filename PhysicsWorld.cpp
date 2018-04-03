/*********************************************
 *	@file		PhysicsWorld.cpp
 *	@brief		�������E ����
 *	@date		2018/03/21
 *	@name		hatakeyama
 ********************************************/


//preprocessor-------------------------------
#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include "Fbx.h"
#include "MyDebugDraw.h"


/**********************************************
 *	@brief		�R���X�g���N�^
 **********************************************/
PhysicsWorld::PhysicsWorld()
	:m_isActive(true)
	, m_debuger(nullptr)
{
	m_collisionShapes.clear();
	m_physicsBodys.clear();
}





/**********************************************
 *	@brief		�f�X�g���N�^
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
 *	@brief			����������
 *	@return			true: ����������	false:���������s
 *	@remarks		1.�Փˌ��o���@�̑I�� �� Default�ɐݒ�
 *					2.�Փ˔���Ď��̐ݒ�
 *					3.�u���[�h�t�F�[�Y�̐ݒ�
 *					4.�S���̃\���o�ݒ�
 *					5.�������E�ݒ�
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
 *	@brief			�������E�̍X�V�E�O�͂̌v�Z
 *	@param[dt]		�ŏI�t���[�����s����
 *	@remarks		�d�͂̌v�Z,�����Ĉʒu�̈ړ�
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
		
		
		//-----World�s�� �擾-----
		trans = obj->getWorldTransform();
		

		//-----���Z���ʂ��擾�E�K�p-----
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
 *	@brief		DebugMode�̑���ŕK�v�ɂȂ鍄�̂̍X�V
 *	@remarks	PhysicsUpdate �� �v�Z�𒆐S��Object���ړ�
 *				DebugPhysicsUpdate �� �N���C�A���g�̈ʒu�𒆐S��Object���ړ�
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
 *	@brief		���̏�񂪐������ݒ肳��Ă��邩�̊m�F
 ***********************************************************************************/
void PhysicsWorld::CheckPhysicsBody()
{
	for (int i = 0; i < m_physicsBodys.size(); i++)
	{
		if (m_physicsBodys[i]->GetTarget() == nullptr)
		{
			OutputDebugString("FBX�ɃZ�b�g����Ă��Ȃ�PhysicsBody������܂�\n");
		}
	}
}
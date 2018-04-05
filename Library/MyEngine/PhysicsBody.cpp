/*****************************************************
 *	@brief		PhysicsBody.cpp
 *	@brief		���̏��̐ݒ������Class ��`
 *	@date		2018/03/21
 *	@name		hatakeyama
 ****************************************************/

//preprocessor-------------------------------------
#include "PhysicsBody.h"
#include "PhysicsWorld.h"
#include "Scene.h"
#include "Fbx.h"


/************************************************************
 *	@brief		�R���X�g���N�^
 ************************************************************/
PhysicsBody::PhysicsBody()
	:m_mass(0)
	, m_restitution(0)
	, m_rigidBody(nullptr)
{
}




/************************************************************
 *	@brief		�f�X�g���N�^
 ************************************************************/
PhysicsBody::~PhysicsBody()
{

}





/************************************************************
 *	@brief		���̏�����
 ************************************************************/
void PhysicsBody::InitWithBody(PhysicsWorld* physicsWorld, D3DXVECTOR3 position, btCollisionShape* collShape)
{


	m_rigidBody = CreateRigidBody(position, collShape);

	physicsWorld->m_collisionShapes.push_back(collShape);
	physicsWorld->m_physicsBodys.push_back(this);
	physicsWorld->m_dynamicsWorld->addRigidBody(m_rigidBody);

}




/************************************************************
 *	@brief		Commmponent������
 ************************************************************/
void PhysicsBody::Init(Node* attachNode)
{
	m_attachNode = attachNode;
	m_typeName = "PhysicsBody";
}



/******************************************************************************************
 *	@brief				RigidBody�̍쐬					-	Create RigidBody.
 *	@param[position]	�ʒu�p��							-	StartTransform
 *	@param[collShape]	����	�͈�							-	PhysicsBody Area
 *	@return				RigidBody�C���X�^���X				-	RigidBody instance.
 *	@remarks			�ݒ���e�F����,���I�ÓIFlag,�������[�����g,�ʒu�p��,�`��
 *****************************************************************************************/
btRigidBody* PhysicsBody::CreateRigidBody(D3DXVECTOR3 position, btCollisionShape* collShape)
{
	btTransform startTransform;

	startTransform.setIdentity();

	//����
	btScalar mass(m_mass);

	//���I�ÓIFlag
	bool isDynamic = (mass != 0.f);

	//�������[�����g
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collShape->calculateLocalInertia(mass, localInertia);

	//�ʒu�p��
	startTransform.setOrigin(btVector3(position.x, position.y, position.z));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collShape, localInertia);

	return new btRigidBody(rbInfo);
}




/******************************************************************************************
 *	@brief			PhysicsBody�C���X�^���X�쐬		-	Create instance of PhysicsBody.
 *	@param[world]	Scene�������Ă���PhysicsWorld		-	PhysicsWorld in scene.
 *	@param[target]	���̂�t������^�[�Q�b�g			-	Add PhysicsBody target
 *	@param[m]		����								-	Mass
 *	@return			PhysicsBody�C���X�^���X			-	PhysicsBody instance.
 *	@remarks		�C���X�^���X�����Ɏ��s�����nullptr���߂�l�ɂȂ�B
 *					Box��p�B
 *****************************************************************************************/
PhysicsBody* PhysicsBody::CreateBox(PhysicsWorld* world, Fbx* target, float m, float sizeRate)
{
	PhysicsBody* ret = new PhysicsBody();

	if (ret)
	{
		target->SetPhysicsBody(ret);
		ret->m_target = target;
		ret->m_mass = m;
		//btCollisionShape* colShape = new btCapsuleShape(btScalar(1), 1);
		btCollisionShape* colShape = new btBoxShape(btVector3(target->m_transform->m_scale.x, target->m_transform->m_scale.y, target->m_transform->m_scale.z) * sizeRate);
		ret->InitWithBody(world, target->m_transform->m_position, colShape);
		ret->Init(target);
		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;
}




/******************************************************************************************
 *	@brief			PhysicsBody�C���X�^���X�쐬		-	Create instance of PhysicsBody.
 *	@param[world]	Scene�������Ă���PhysicsWorld		-	PhysicsWorld in scene.
 *	@param[target]	���̂�t������^�[�Q�b�g			-	Add PhysicsBody target
 *	@param[m]		����								-	Mass
 *	@return			PhysicsBody�C���X�^���X			-	PhysicsBody instance.
 *	@remarks		�C���X�^���X�����Ɏ��s�����nullptr���߂�l�ɂȂ�B
 *					Sphere��p
 *****************************************************************************************/
PhysicsBody* PhysicsBody::CreateSphere(PhysicsWorld* world, Fbx* target, float m, float sizeRate)
{
	PhysicsBody* ret = new PhysicsBody();

	if (ret)
	{
		target->SetPhysicsBody(ret);
		ret->m_target = target;
		ret->m_mass = m;
		btCollisionShape* colShape = new btSphereShape(btScalar(target->m_transform->m_scale.x)* sizeRate);
		ret->InitWithBody(world, target->m_transform->m_position, colShape);
		ret->Init(target);

		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;
}




/******************************************************************************************
 *	@brief			���ʂ̐ݒ��ύX����				-	Change Mass
 *	@param[mass]	�ύX��̎���						-	Change After Mass
 *	@remarks		0��ݒ肷��ƐÓI���̂ɂȂ�
 *****************************************************************************************/
void PhysicsBody::SetMass(float mass)
{
	if (m_target == nullptr)
	{
		R_ASSERT(m_target);
	}

	m_mass = mass;

	D3DXVECTOR3 pos = m_target->m_transform->m_position;

	PhysicsWorld* world = dynamic_cast<Scene*>(m_target->m_transform->m_parent->m_attachNode)->GetPhysicsWorld();

	btCollisionShape* shape = m_rigidBody->getCollisionShape();

	world->m_dynamicsWorld->removeRigidBody(m_rigidBody);

	m_rigidBody = CreateRigidBody(pos, shape);

	world->m_dynamicsWorld->addRigidBody(m_rigidBody);
}
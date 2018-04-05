/*****************************************************
 *	@brief		PhysicsBody.cpp
 *	@brief		剛体情報の設定をするClass 定義
 *	@date		2018/03/21
 *	@name		hatakeyama
 ****************************************************/

//preprocessor-------------------------------------
#include "PhysicsBody.h"
#include "PhysicsWorld.h"
#include "Scene.h"
#include "Fbx.h"


/************************************************************
 *	@brief		コンストラクタ
 ************************************************************/
PhysicsBody::PhysicsBody()
	:m_mass(0)
	, m_restitution(0)
	, m_rigidBody(nullptr)
{
}




/************************************************************
 *	@brief		デストラクタ
 ************************************************************/
PhysicsBody::~PhysicsBody()
{

}





/************************************************************
 *	@brief		剛体初期化
 ************************************************************/
void PhysicsBody::InitWithBody(PhysicsWorld* physicsWorld, D3DXVECTOR3 position, btCollisionShape* collShape)
{


	m_rigidBody = CreateRigidBody(position, collShape);

	physicsWorld->m_collisionShapes.push_back(collShape);
	physicsWorld->m_physicsBodys.push_back(this);
	physicsWorld->m_dynamicsWorld->addRigidBody(m_rigidBody);

}




/************************************************************
 *	@brief		Commmponent初期化
 ************************************************************/
void PhysicsBody::Init(Node* attachNode)
{
	m_attachNode = attachNode;
	m_typeName = "PhysicsBody";
}



/******************************************************************************************
 *	@brief				RigidBodyの作成					-	Create RigidBody.
 *	@param[position]	位置姿勢							-	StartTransform
 *	@param[collShape]	剛体	範囲							-	PhysicsBody Area
 *	@return				RigidBodyインスタンス				-	RigidBody instance.
 *	@remarks			設定内容：質量,動的静的Flag,慣性モーメント,位置姿勢,形状
 *****************************************************************************************/
btRigidBody* PhysicsBody::CreateRigidBody(D3DXVECTOR3 position, btCollisionShape* collShape)
{
	btTransform startTransform;

	startTransform.setIdentity();

	//質量
	btScalar mass(m_mass);

	//動的静的Flag
	bool isDynamic = (mass != 0.f);

	//慣性モーメント
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collShape->calculateLocalInertia(mass, localInertia);

	//位置姿勢
	startTransform.setOrigin(btVector3(position.x, position.y, position.z));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collShape, localInertia);

	return new btRigidBody(rbInfo);
}




/******************************************************************************************
 *	@brief			PhysicsBodyインスタンス作成		-	Create instance of PhysicsBody.
 *	@param[world]	Sceneが持っているPhysicsWorld		-	PhysicsWorld in scene.
 *	@param[target]	剛体を付属するターゲット			-	Add PhysicsBody target
 *	@param[m]		質量								-	Mass
 *	@return			PhysicsBodyインスタンス			-	PhysicsBody instance.
 *	@remarks		インスタンス生成に失敗するとnullptrが戻り値になる。
 *					Box専用。
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
 *	@brief			PhysicsBodyインスタンス作成		-	Create instance of PhysicsBody.
 *	@param[world]	Sceneが持っているPhysicsWorld		-	PhysicsWorld in scene.
 *	@param[target]	剛体を付属するターゲット			-	Add PhysicsBody target
 *	@param[m]		質量								-	Mass
 *	@return			PhysicsBodyインスタンス			-	PhysicsBody instance.
 *	@remarks		インスタンス生成に失敗するとnullptrが戻り値になる。
 *					Sphere専用
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
 *	@brief			質量の設定を変更する				-	Change Mass
 *	@param[mass]	変更後の質量						-	Change After Mass
 *	@remarks		0を設定すると静的剛体になる
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
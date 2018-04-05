/*********************************************************************
 *	@file		PhysicsBody.h
 *	@brief		剛体の実装
 *	@date		2018/02/19
 *	@name		hatakeyama
 *********************************************************************/


#ifndef __PHYSICSBODY__
#define __PHYSICSBODY__

//preprocessor----------------------------------------
#include "Global.h"
#include "btBulletDynamicsCommon.h"
#include "Commponent.h"


//prototype-------------------------------------------
class Fbx;
class PhysicsWorld;



class PhysicsBody : public Component
{

//variable---------------------------------------------------
private:
	float m_mass;
	float m_restitution;

	D3DXVECTOR3 m_impulse;
	D3DXVECTOR3 m_force;


	D3DXVECTOR3 m_collisionOrigin;

	btRigidBody* m_rigidBody;

	Fbx* m_target;



//function----------------------------------------------------------------------------------------------
public:
	PhysicsBody();

	~PhysicsBody();

	void InitWithBody(PhysicsWorld* physicsWorld,D3DXVECTOR3 position,btCollisionShape* collShape);
	void Init(Node* attachNode);

	/*************************************************************
	 *	@brief			質量のセッター
	 *************************************************************/
	void SetMass(float mass);


	/*************************************************************
	 *	@brief			質量のセッター
	 *************************************************************/
	inline void SetImpulse(D3DXVECTOR3 vector) { m_impulse = vector; }



	/*************************************************************
	 *	@brief			反発係数のセッター
	 *************************************************************/
	inline void SetRestitutuion(float restitution) { m_restitution = restitution; }


	inline void SetFbx(Fbx* fbx) { m_target = fbx; }

	void SetRigidBody(btRigidBody* body) { m_rigidBody = body; }


	/*************************************************************
	 *	@brief			質量ゲッター
	 *************************************************************/
	inline float GetMass() const { return m_mass; }


	/*************************************************************
	*	@brief			剛体のターゲット取得
	*************************************************************/
	inline Fbx* GetTarget() const { return m_target; }


	inline btRigidBody* GetRigidBody() { return m_rigidBody; }


	btRigidBody* CreateRigidBody(D3DXVECTOR3 position, btCollisionShape* collShape);

	static PhysicsBody* CreateBox(PhysicsWorld* world, Fbx* target , float m = 0, float sizeRate = 0.5f);


	static PhysicsBody* CreateSphere(PhysicsWorld* world, Fbx* target, float m = 0, float sizeRate = 0.5f);


};

#endif //!__PHYSICSBODY__
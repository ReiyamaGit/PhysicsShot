/*********************************************************************
 *	@file		PhysicsBody.h
 *	@brief		���̂̎���
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
	 *	@brief			���ʂ̃Z�b�^�[
	 *************************************************************/
	void SetMass(float mass);


	/*************************************************************
	 *	@brief			���ʂ̃Z�b�^�[
	 *************************************************************/
	inline void SetImpulse(D3DXVECTOR3 vector) { m_impulse = vector; }



	/*************************************************************
	 *	@brief			�����W���̃Z�b�^�[
	 *************************************************************/
	inline void SetRestitutuion(float restitution) { m_restitution = restitution; }


	inline void SetFbx(Fbx* fbx) { m_target = fbx; }

	void SetRigidBody(btRigidBody* body) { m_rigidBody = body; }


	/*************************************************************
	 *	@brief			���ʃQ�b�^�[
	 *************************************************************/
	inline float GetMass() const { return m_mass; }


	/*************************************************************
	*	@brief			���̂̃^�[�Q�b�g�擾
	*************************************************************/
	inline Fbx* GetTarget() const { return m_target; }


	inline btRigidBody* GetRigidBody() { return m_rigidBody; }


	btRigidBody* CreateRigidBody(D3DXVECTOR3 position, btCollisionShape* collShape);

	static PhysicsBody* CreateBox(PhysicsWorld* world, Fbx* target , float m = 0, float sizeRate = 0.5f);


	static PhysicsBody* CreateSphere(PhysicsWorld* world, Fbx* target, float m = 0, float sizeRate = 0.5f);


};

#endif //!__PHYSICSBODY__
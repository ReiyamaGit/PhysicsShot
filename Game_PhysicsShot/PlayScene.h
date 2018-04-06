/*************************************************************
 *	@file	PlayScene.h
 *	@brief	DemoScene ��`
 *	@date	2018/03/21
 *	@name	hatakeyama
 *************************************************************/
#ifndef __PLAYSCENE__
#define __PLAYSCENE__


//preprocessor-----------------------------------------------
#include "DebugScene.h"
#include <vector>


//prototype---------------------------------------------------
class Quad;
class Fbx;
class btDiscreteDynamicsWorld;



/************************************************************
 *	@brief			Demo
 ************************************************************/

class PlayScene : public DebugScene
{
private:
	Fbx* m_torus;
	
	

public:
	//�R���X�g���N�^
	PlayScene();

	//�f�X�g���N�^
	~PlayScene();

public:

	//�������֐�
	bool Init() override;

	//�X�V�֐�
	bool Update(float delta) override;

	//���͊֐�
	void Input();

	static PlayScene* CreateWithPhysicsWorld();

	static PlayScene* Create();
};

#endif //!__PLAYSCENE


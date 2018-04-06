/*************************************************************
 *	@file	PlayScene.h
 *	@brief	DemoScene 定義
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
	//コンストラクタ
	PlayScene();

	//デストラクタ
	~PlayScene();

public:

	//初期化関数
	bool Init() override;

	//更新関数
	bool Update(float delta) override;

	//入力関数
	void Input();

	static PlayScene* CreateWithPhysicsWorld();

	static PlayScene* Create();
};

#endif //!__PLAYSCENE



/******************************************************
 *	@file		Fbx.h
 *	@brief		Fbx 定義
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#ifndef __GAME__
#define __GAME__

//preprocessor-----------------------------
#include "Global.h"
#include "Scene.h"


/**************************
 *	@brief	Game Class
 **************************/
class Game
{


//variable------------------------
private:

	LPDIRECT3D9 _pD3d;     


//function------------------------
public:

	/*********************************************************
	 *	@brief			コンストラクタ
	 *********************************************************/
	Game();		

	
	/*********************************************************
	 *	@brief			デストラクタ
	 *********************************************************/
	~Game();	


	
	/*********************************************************
	 *	@brief			描画設定などの初期化
	 *********************************************************/
	void Init(HWND hWnd);	


	/*********************************************************
	 *	@brief			ゲーム開始時,一部設定のチェック
	 *********************************************************/
	void InitCheck();


	/*********************************************************
	 *	@brief			描画設定などの初期化
	 *********************************************************/
	void Update(float delta);	


	/*********************************************************
	 *	@brief			Game全体としての入力処理
	 *********************************************************/
	void Input();	


	/*********************************************************
	 *	@brief			Gameの描画
	 *********************************************************/
	void Draw();	


};

#endif // !__GAME__

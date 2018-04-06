/***************************************************************
 *	@file				DebugScene.h
 *	@brief				DebugScene Class 定義
 *	@date				2018/01/24
 *	@name				hatakeyama
 ***************************************************************/

#ifndef __DEBUGSCENE__
#define __DEBUGSCENE__


//preprocessor--------------------------------------------------
#include "Scene.h"


//prototype----------------------------------------------------
class Node;
class Label;
class ToggleButton;



/***************************************************************
 *	@brief  パラメーターとかを表示する専用Scene Class
 ***************************************************************/
class DebugScene : public Scene
{
//variable---------------------------------------------------------------
private:
	bool m_isEdit;
	float m_camZoomPos;

	
	Label* m_xPos;								//ItemのX座標
	Label* m_yPos;								//ItemのY座標
	Label* m_zPos;								//ItemのZ座標
	Label* m_xRotation;							//ItemのX軸角度
	Label* m_yRotation;							//ItemのY軸角度
	Label* m_zRotation;							//ItemnoZ軸角度
	Label* m_fps;

	ToggleButton* m_editButton;					//編集モードボタン
	
	std::vector<Node*> m_debugItemList;


	D3DXVECTOR3 m_GameCamPos;
	D3DXVECTOR2 m_SceneCamPos;
	D3DXVECTOR2 m_touchBeginPos;

protected:
	Node*  m_selectedItem;						//選択したItem	

//function-----------------------------------------------------------------
public:

	DebugScene();

	~DebugScene();


	/******************************************************
	 *	@brief			初期化処理
	 ******************************************************/
	virtual bool Init();
	
	
	/******************************************************
	 *	@brief			更新処理
	 *	@param[delta]	最終フレーム実行時間
	 ******************************************************/
	virtual bool Update(float delta);


	/******************************************************
	 *	@brief			入力処理
	 ******************************************************/
	virtual void Input();



	/******************************************************
	 *	@brief			選択したItemの回転
	 ******************************************************/
	void ItemUpDown();
	

	/******************************************************
	 *	@brief			スクリーンからオブジェクトを選ぶ
	 ******************************************************/
	bool SelectObjectFromScreen();


	
	/******************************************************
	 *	@brief			クリックしたところにオブジェクトを作成
	 ******************************************************/
	void CreateInSelectedPoint();


	
	/******************************************************
	 *	@brief			編集ボタンの作成
	 ******************************************************/
	void CreateEditButton();
	
	
	/******************************************************
	 *	@brief			DebugシーンのON・OFF
	 ******************************************************/
	void SetActive(bool active);


	
};


#endif //!__DEBUGSCENE__
/*************************************************************
 *	@file			Touch.h
 *	@brief			Touchイベント検知
 *	@date			2018/01/30
 *	@name			hatakeyama
 ************************************************************/


#ifndef __TOUCHLISTENER__
#define __TOUCHLISTENER__


//preprocessor-------------------------------------------------
#include "Node.h"
#include <functional>


class TouchListener : public Node
{

//struct-------------------------------------------------------
public:
	typedef struct Touch
	{
		D3DXVECTOR2 position;


		/*******************************************************
		 *	@brief		コンストラクタ
		 *******************************************************/
		Touch()
		{
			position.x = GLOBAL::Global::m_input->GetMousePosition().x;
			position.y = GLOBAL::Global::m_input->GetMousePosition().y;
		}
	};


//variable-----------------------------------------------------
public:
	typedef std::function<bool(Touch)>  onCallBackBegan;
	typedef std::function<void(Touch)>  onCallBackMove;
	typedef std::function<void(Touch)>  onCallBackEnd;

	onCallBackBegan onTouchBegan;
	onCallBackMove  onTouchMove;
	onCallBackEnd   onTouchEnd;


//function------------------------------------------------------
public:

	TouchListener();

	~TouchListener();

	/***********************************************************
	 *	@brief			初期化処理
	 ***********************************************************/
	bool Init() override;


	/********************************************************************
	 *	@brief			マウスの入力状態ごとの関数で設定されているものを実行
	 *******************************************************************/
	void Input();

	/***********************************************************
	 *	@brief			インスタンス生成
	 ***********************************************************/
	static TouchListener* Create();


};




#endif //!__TOUCHLISTENER__

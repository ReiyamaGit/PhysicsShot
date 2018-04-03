/******************************************************
 *	@file		ToggleButton.cpp
 *	@brief		ToggleButton 実装
 *	@data		2018/01/18
 *	@name		hatakeyama
 ******************************************************/




//preprocessor----------------------------------------
#include "ToggleButton.h"
#include "TouchListener.h"




/********************************************
 *	@brief		コンストラクタ
 ********************************************/
ToggleButton::ToggleButton()
{
	m_nodeName = "ToggleButton";
}


/********************************************
*	@brief		デストラクタ
********************************************/
ToggleButton::~ToggleButton()
{
	
}



/********************************************
 *	@brief		ToggleButtonの作成
 *	@remarks	AutoRelease対象
 ********************************************/
ToggleButton* ToggleButton::Create(LPCSTR normal,LPCSTR pushed)
{
	ToggleButton* ret = new ToggleButton();

	if (ret)
	{
		ret->Load(normal, pushed);
		return ret;
	}

	R_SAFE_DELETE(ret);

	return ret;
}




/*************************************************
 *	@brief			ToggleButtonの作成・動作に必要な情報
 *	@param[normal]	デフォルト画像	
 *	@param[pushed]	選択したときの画像
 *************************************************/
void ToggleButton::Load(LPCSTR normal, LPCSTR pushed)
{

	Sprite::Load(normal);


	m_normal = normal;
	m_pushed = pushed;

	Input();

}




/********************************************************
 *	@brief			更新処理
 *	@param[delta]	最終フレーム実行時間
 *	@return			true	:	更新処理全過程終了
 *					false	:	更新処理中断
 ********************************************************/
bool ToggleButton::Update(float delta)
{
	if (!Node::Update(delta))
	{
		return false;
	}


	return true;
}





/********************************************************
 *	@brief		入力処理
 *	@return		true	:	入力処理全過程終了
 *				false	:	入力処理中断
 ********************************************************/
void ToggleButton::Input()
{
	TouchListener* listener = TouchListener::Create();

	listener->onTouchBegan = [this](TouchListener::Touch touch)
	{

		if (GLOBAL::Global::m_input->IsMousePush(0)
			&& CheckCursorOnToggleButton()
			&& m_isActive)
		{

			if (this->GetSpriteName() == m_normal)
			{
				this->SetSprite(m_pushed);
			}
			else if (this->GetSpriteName() == m_pushed)
			{

				this->SetSprite(m_normal);
			}

			
			onTouchBegan();
			
		}

		return true;
	};

	GLOBAL::Global::m_eventDispatcher->AddEventListener(listener);

}



/**********************************************
 *	@brief		Button上にカーソルがあるか検知
 **********************************************/
bool ToggleButton::CheckCursorOnToggleButton()
{
	if (this->GetBoudingBox2D().ContainsPoint(GLOBAL::Global::m_input->GetMousePosition()))
	{
		return true;
	}

	return false;
}




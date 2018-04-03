/************************************************************
 *	@file		EventDispatcher.cpp
 *	@brief		EventDispatcher -> TouchListnerをまとめる役
 *	@date		2018/03/19
 *	@name		hatakeyama
 ************************************************************/

//preprocessor
#include "EventDispatcher.h"
#include "Global.h"
#include "TouchListener.h"


/************************************************************
 *	@brief		コンストラクタ	
 ************************************************************/
EventDispatcher::EventDispatcher()
{
	m_referenceCount = 1;
	m_registerEvents.clear();
}




/************************************************************
 *	@brief		デストラクタ
 ************************************************************/
EventDispatcher::~EventDispatcher()
{
	//m_registerEvents.shrink_to_fit();
	for (int i = 0; i < m_registerEvents.size(); i++)
	{
		m_registerEvents[i]->CountRelease();
	}
}



/************************************************************
 *	@brief		インスタンス生成
 ************************************************************/
EventDispatcher* EventDispatcher::Create()
{
	EventDispatcher* ret = new EventDispatcher();

	if (ret)
	{
		//---リファレンスカウンタの対象に設定
		ret->AutoRelease();
		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;
	
}



 /************************************************************
  *	@brief					新規リスナーの登録
  *	@param[listener]		新規リスナー
  ************************************************************/
void EventDispatcher::AddEventListener(TouchListener* listener)
{
	m_registerEvents.push_back(listener);
}



/*********************************************
 *		@brief					イベントの消去
 *		@param[index]			イベントの番号
 *********************************************/
void EventDispatcher::DeleteEvent(int index)
{
	m_registerEvents.erase(m_registerEvents.begin() + index);
}


/*********************************************
 *	@brief	登録されたイベントの実行
 *********************************************/
void EventDispatcher::Input()
{
	for (int i = 0; i < m_registerEvents.size(); i++)
	{
		m_registerEvents[i]->Input();
	}
}




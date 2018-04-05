
/*******************************************************************
 *	@file		EventDispatcher.h
 *	@brief		イベント監視役
 *	@date		2018/02/06
 *	@name		hatakeyama
 *******************************************************************/

#ifndef __EVENTDISPATCHER__
#define __EVENTDISPATCHER__


//preprocessor------------------------------------------------
#include "Ref.h"
#include <vector>


//prototype---------------------------------------------------
class TouchListener;



class EventDispatcher :	public Ref
{

//variable-----------------------------------------------------
private:
	std::vector<TouchListener*> m_registerEvents;


//function----------------------------------------------------
public:
	
	EventDispatcher();
	

	~EventDispatcher();



	/*********************************************
	 *	@brief					新規イベントの登録
	 *	@param[listener]		新規イベント
	 *********************************************/
	void AddEventListener(TouchListener*  listener);

	

	/*********************************************
	 *	@brief					イベントの消去
	 *	@param[index]			イベントの番号
	 *********************************************/
	void DeleteEvent(int index);


	/*********************************************
	 *	@brief	登録されたイベントの実行
	 *********************************************/
	void Input();



	/*********************************************
	 *	@brief	インスタンス生成
	 *********************************************/
	static EventDispatcher* Create();
};


#endif //!__EVENTDISPATCHER__


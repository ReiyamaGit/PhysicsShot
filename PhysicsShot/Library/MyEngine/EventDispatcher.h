
/*******************************************************************
 *	@file		EventDispatcher.h
 *	@brief		�C�x���g�Ď���
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
	 *	@brief					�V�K�C�x���g�̓o�^
	 *	@param[listener]		�V�K�C�x���g
	 *********************************************/
	void AddEventListener(TouchListener*  listener);

	

	/*********************************************
	 *	@brief					�C�x���g�̏���
	 *	@param[index]			�C�x���g�̔ԍ�
	 *********************************************/
	void DeleteEvent(int index);


	/*********************************************
	 *	@brief	�o�^���ꂽ�C�x���g�̎��s
	 *********************************************/
	void Input();



	/*********************************************
	 *	@brief	�C���X�^���X����
	 *********************************************/
	static EventDispatcher* Create();
};


#endif //!__EVENTDISPATCHER__


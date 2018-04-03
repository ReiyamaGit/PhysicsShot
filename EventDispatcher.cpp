/************************************************************
 *	@file		EventDispatcher.cpp
 *	@brief		EventDispatcher -> TouchListner���܂Ƃ߂��
 *	@date		2018/03/19
 *	@name		hatakeyama
 ************************************************************/

//preprocessor
#include "EventDispatcher.h"
#include "Global.h"
#include "TouchListener.h"


/************************************************************
 *	@brief		�R���X�g���N�^	
 ************************************************************/
EventDispatcher::EventDispatcher()
{
	m_referenceCount = 1;
	m_registerEvents.clear();
}




/************************************************************
 *	@brief		�f�X�g���N�^
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
 *	@brief		�C���X�^���X����
 ************************************************************/
EventDispatcher* EventDispatcher::Create()
{
	EventDispatcher* ret = new EventDispatcher();

	if (ret)
	{
		//---���t�@�����X�J�E���^�̑Ώۂɐݒ�
		ret->AutoRelease();
		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;
	
}



 /************************************************************
  *	@brief					�V�K���X�i�[�̓o�^
  *	@param[listener]		�V�K���X�i�[
  ************************************************************/
void EventDispatcher::AddEventListener(TouchListener* listener)
{
	m_registerEvents.push_back(listener);
}



/*********************************************
 *		@brief					�C�x���g�̏���
 *		@param[index]			�C�x���g�̔ԍ�
 *********************************************/
void EventDispatcher::DeleteEvent(int index)
{
	m_registerEvents.erase(m_registerEvents.begin() + index);
}


/*********************************************
 *	@brief	�o�^���ꂽ�C�x���g�̎��s
 *********************************************/
void EventDispatcher::Input()
{
	for (int i = 0; i < m_registerEvents.size(); i++)
	{
		m_registerEvents[i]->Input();
	}
}




/******************************************************
 *	@file		ToggleButton.cpp
 *	@brief		ToggleButton ����
 *	@data		2018/01/18
 *	@name		hatakeyama
 ******************************************************/




//preprocessor----------------------------------------
#include "ToggleButton.h"
#include "TouchListener.h"




/********************************************
 *	@brief		�R���X�g���N�^
 ********************************************/
ToggleButton::ToggleButton()
{
	m_nodeName = "ToggleButton";
}


/********************************************
*	@brief		�f�X�g���N�^
********************************************/
ToggleButton::~ToggleButton()
{
	
}



/********************************************
 *	@brief		ToggleButton�̍쐬
 *	@remarks	AutoRelease�Ώ�
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
 *	@brief			ToggleButton�̍쐬�E����ɕK�v�ȏ��
 *	@param[normal]	�f�t�H���g�摜	
 *	@param[pushed]	�I�������Ƃ��̉摜
 *************************************************/
void ToggleButton::Load(LPCSTR normal, LPCSTR pushed)
{

	Sprite::Load(normal);


	m_normal = normal;
	m_pushed = pushed;

	Input();

}




/********************************************************
 *	@brief			�X�V����
 *	@param[delta]	�ŏI�t���[�����s����
 *	@return			true	:	�X�V�����S�ߒ��I��
 *					false	:	�X�V�������f
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
 *	@brief		���͏���
 *	@return		true	:	���͏����S�ߒ��I��
 *				false	:	���͏������f
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
 *	@brief		Button��ɃJ�[�\�������邩���m
 **********************************************/
bool ToggleButton::CheckCursorOnToggleButton()
{
	if (this->GetBoudingBox2D().ContainsPoint(GLOBAL::Global::m_input->GetMousePosition()))
	{
		return true;
	}

	return false;
}




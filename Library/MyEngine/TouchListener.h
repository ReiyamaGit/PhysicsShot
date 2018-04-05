/*************************************************************
 *	@file			Touch.h
 *	@brief			Touch�C�x���g���m
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
		 *	@brief		�R���X�g���N�^
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
	 *	@brief			����������
	 ***********************************************************/
	bool Init() override;


	/********************************************************************
	 *	@brief			�}�E�X�̓��͏�Ԃ��Ƃ̊֐��Őݒ肳��Ă�����̂����s
	 *******************************************************************/
	void Input();

	/***********************************************************
	 *	@brief			�C���X�^���X����
	 ***********************************************************/
	static TouchListener* Create();


};




#endif //!__TOUCHLISTENER__

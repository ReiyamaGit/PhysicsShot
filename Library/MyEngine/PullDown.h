/***********************************************************
 *	@file		PullDown.h
 *	@brief		PullDown ����
 *	@date		2018/01/29
 *	@name		hatakeyama
 **********************************************************/

#ifndef __PULLDOWN__
#define __PULLDOWN__


//preprocessor--------------------------------------------
#include "Sprite.h"
#include <vector>
#include <functional>




class PullDown : public Node
{
//variable----------------------------------------------
private:
	std::vector<Sprite*> m_sprites;

	bool m_isOpen;
	bool m_isAnimation;

	int m_currentIndex;

	int  m_openButtonSize;

	LPCSTR m_mainSpriteName;

//function-----------------------------------------------
public:

	PullDown();

	~PullDown();

	/************************************************
	 *	@brief				PullDown�̃C���X�^���X����
	 *	@param[sprites]		�����ɒǉ�����摜
	 ************************************************/
	static PullDown* Create(Sprite* sprites);



	/********************************************
	 *	@brief				PullDown���X�g�ɒǉ�
	 *	@param[sprite]		�ǉ�����摜
	 ********************************************/
	void AddList(Sprite* sprite);



	/*****************************************
	 *	@brief				�v�f�̍폜
	 *	@param[index]		�폜����v�f�̔ԍ�
	 *	@remarks			�ԍ���0����X�^�[�g
	 *****************************************/
	void RemoveAtIndex(int index);


	/******************************************************
	 *	@brief			PullDownAnimation����
	 *******************************************************/
	void ButtonAnimation();

	void SetItemPosition();


	/**********************************************************************************
	 *	@brief				����������
	 **********************************************************************************/
	void Init(Sprite* sprite);

	bool Update(float delta) override;


	/**********************************************************************************
	 *	@brief				���͏��� ���܂�������
	 **********************************************************************************/
	void Input();



	/**********************************************************************************
	 *	@brief					�摜��؂�ւ���
	 *	@param[mainSprite]		�ォ��ǉ�����摜
	 **********************************************************************************/
	void SwitchMainImage(bool isClick);
};

#endif //!__PULLDOWN__


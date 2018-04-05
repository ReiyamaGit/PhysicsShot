/********************************************************
 *	@file		PullDown.cpp
 *	@brief		PullDownList���쐬����Class ����
 *	@date		2018/03/21
 *	@name		hatakeyama
 *******************************************************/



//preprocessor----------------------------------
#include "PullDown.h"
#include "TouchListener.h"


/************************************************
 *	@brief			�R���X�g���N�^
 ************************************************/
PullDown::PullDown()
: m_isDown(false)
{
	m_sprites.clear();
}


/************************************************
 *	@brief			�f�X�g���N�^
 ************************************************/
PullDown::~PullDown()
{
	m_sprites.shrink_to_fit();
}




/************************************************
 *	@brief				PullDown�̃C���X�^���X����
 *	@param[sprites]		�����ɒǉ�����摜
 ************************************************/
PullDown* PullDown::Create(Sprite* sprites)
{
	PullDown* ret = new PullDown();



	if (ret)
	{
		ret->Init(sprites);
		ret->AutoRelease();
		return ret;
	}

	R_SAFE_DELETE(ret);


	return nullptr;
}




/********************************************
 *	@brief				PullDown���X�g�ɒǉ�
 *	@param[sprite]		�ǉ�����摜
 ********************************************/
void PullDown::AddList(Sprite* sprite)
{
	sprite->m_transform->m_position = this->m_transform->m_position;
	sprite->m_transform->SetParent(this->m_transform);
	m_sprites.push_back(sprite);
	SetItemPosition();
}




/**********************************************************************************
 *	@brief				�v�f�̍폜
 *	@param[index]		�폜����v�f�̔ԍ�
 *	@remarks			�ԍ���0����X�^�[�g
 **********************************************************************************/
void PullDown::RemoveAtIndex(int index)
{

	R_SAFE_COUNT_RELEASE(m_sprites[index]);
	m_sprites.erase(m_sprites.begin() + index);
}



/**********************************************************************************
 *	@brief				����������
 *	@param[sprite]		�ŏ��ɕ\������摜
 *	@remarks			�ԍ���0����X�^�[�g
 **********************************************************************************/
void PullDown::Init(Sprite* sprite)
{

	sprite->m_transform->m_position = this->m_transform->m_position;
	sprite->m_transform->SetParent(this->m_transform);
	m_sprites.push_back(sprite);

}





/**********************************************************************************
 *	@brief				����������
 *	@param[sprite]		�ŏ��ɕ\������摜
 *	@remarks			�ԍ���0����X�^�[�g
 **********************************************************************************/
void PullDown::SetItemPosition()
{

	for (int i = 1; i < m_sprites.size(); i++)
	{
		m_sprites[i]->m_transform->m_position = D3DXVECTOR3(0, i * m_sprites[i - 1]->m_transform->m_size.y, 0);
	}
}




/**********************************************************************************
 *	@brief				���͏��� ���܂�������
 **********************************************************************************/
void PullDown::Input()
{
	
}





/**********************************************************************************
 *	@brief					�摜��؂�ւ���
 *	@param[mainSprite]		�ォ��ǉ�����摜
 **********************************************************************************/
void PullDown::MainSpriteChange(Sprite* mainSprite)
{

	for (int i = 0; i < m_sprites.size(); i++)
	{
		if (mainSprite->GetSpriteName() == m_sprites[i]->GetSpriteName())
		{
			m_sprites.erase(m_sprites.begin() + i);
		}
	}

	m_sprites.insert(m_sprites.begin(), mainSprite);
}
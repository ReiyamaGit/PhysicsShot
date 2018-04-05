/******************************************************
 *	@file		PoolManager.cpp
 *	@brief		PoolManager ����
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "PoolManager.h"



//static variabel initialize---------------------------
PoolManager* PoolManager::pool_manager_ = nullptr;





/****************************************************
 *	@brief	�R���X�g���N�^
 ****************************************************/
PoolManager::PoolManager()
{
	register_object_.clear();
}






/****************************************************
 *	@brief	�f�X�g���N�^
 ****************************************************/
PoolManager::~PoolManager()
{
	register_object_.shrink_to_fit();
}







/********************************************************************
 *	@brief		���t���[�����t�@�����X�J�E���g���`�F�b�N����
 *	@remarks	���t���[�����ׂẴ��t�@�����X�J�E���g���`�F�b�N��,
 *				���t�@�����X�J�E���g��0�̏ꍇ��,�I�u�W�F�N�g���폜����
 *******************************************************************/
void PoolManager::Check()
{
	for (unsigned int i = 0; i < register_object_.size(); i++)
	{
		if (register_object_[i]->GetReferenceCount() <=0 )
		{

			delete register_object_[i];

			register_object_.erase(register_object_.begin() + i);
		}
	}
}







/***************************************************************************
 *	@brief		�I�u�W�F�N�g��ǉ�����
 *	@remarks	�����œn���ꂽ�I�u�W�F�N�g��PoolManager�̊Ǘ����X�g�ɒǉ�����
 ***************************************************************************/
void PoolManager::AddObject(Ref* object)
{
	register_object_.push_back(object);
}




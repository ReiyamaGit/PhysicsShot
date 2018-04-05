/************************************************
 *	@file		PoolManager.cpp	
 *	@brief		���t�@�����X�J�E���^����
 *	@date		2018/03/21
 *	@name		hatakeyama
 ***********************************************/

#ifndef __POOLMANAGER__
#define __POOLMANAGER__


//preprocessor----------------------------------
#include <vector>
#include <string>
#include "Ref.h"




class PoolManager
{
public:
	
	/********************************************************************
	 *	@brief		�C���X�^���X�̎擾
   	 *******************************************************************/
	static PoolManager* GetInstance() { if (!pool_manager_) pool_manager_ = new PoolManager(); return pool_manager_; }


	/********************************************************************
	 *	@brief		���t���[�����t�@�����X�J�E���g���`�F�b�N����
   	 *******************************************************************/
	void Check();
	

	/***************************************************************************
	 *	@brief		�I�u�W�F�N�g��ǉ�����
     ***************************************************************************/
	void AddObject(Ref* late);

	/********************************************************
	 *	@brief		�C���X�^���X�j��
	 ********************************************************/
	void Destroy() { delete pool_manager_; pool_manager_ = nullptr; }


private:

	PoolManager();

	~PoolManager();

	static PoolManager* pool_manager_;	 //�V���O���g���B�C���X�^���X

	std::vector<Ref*> register_object_;  //pool���Ă����z��

	
};

#endif // !__PoolManager__

/******************************************************
 *	@file		PoolManager.cpp
 *	@brief		PoolManager 実装
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "PoolManager.h"



//static variabel initialize---------------------------
PoolManager* PoolManager::pool_manager_ = nullptr;





/****************************************************
 *	@brief	コンストラクタ
 ****************************************************/
PoolManager::PoolManager()
{
	register_object_.clear();
}






/****************************************************
 *	@brief	デストラクタ
 ****************************************************/
PoolManager::~PoolManager()
{
	register_object_.shrink_to_fit();
}







/********************************************************************
 *	@brief		毎フレームリファレンスカウントをチェックする
 *	@remarks	毎フレームすべてのリファレンスカウントをチェックし,
 *				リファレンスカウントが0の場合は,オブジェクトを削除する
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
 *	@brief		オブジェクトを追加する
 *	@remarks	引数で渡されたオブジェクトをPoolManagerの管理リストに追加する
 ***************************************************************************/
void PoolManager::AddObject(Ref* object)
{
	register_object_.push_back(object);
}




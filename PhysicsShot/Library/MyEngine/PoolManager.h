/************************************************
 *	@file		PoolManager.cpp	
 *	@brief		リファレンスカウンタ実装
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
	 *	@brief		インスタンスの取得
   	 *******************************************************************/
	static PoolManager* GetInstance() { if (!pool_manager_) pool_manager_ = new PoolManager(); return pool_manager_; }


	/********************************************************************
	 *	@brief		毎フレームリファレンスカウントをチェックする
   	 *******************************************************************/
	void Check();
	

	/***************************************************************************
	 *	@brief		オブジェクトを追加する
     ***************************************************************************/
	void AddObject(Ref* late);

	/********************************************************
	 *	@brief		インスタンス破壊
	 ********************************************************/
	void Destroy() { delete pool_manager_; pool_manager_ = nullptr; }


private:

	PoolManager();

	~PoolManager();

	static PoolManager* pool_manager_;	 //シングルトン。インスタンス

	std::vector<Ref*> register_object_;  //poolしておく配列

	
};

#endif // !__PoolManager__

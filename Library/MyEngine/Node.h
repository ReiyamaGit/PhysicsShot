/************************************************************
 *	@file		Node.h
 *	@brief		全てのObjectの基本形 定義
 *	@date		2017/06/22
 *	@name		hatakeyama
 ************************************************************/


#ifndef __NODE__
#define __NODE__



//preprocessor------------------------------------------
#include "Global.h"
#include "Ref.h"
#include "Transform.h"
#include <vector>


//prototype---------------------------------------------
class Scene;



class Node:public Ref
{

//variable------------------------------------------------
public:
	bool m_isUpdate;					
	
	bool m_isActive;

	MyRect m_rect;

	LPCSTR m_nodeName;

	Transform* m_transform;

public:

	Node();

	~Node();

	
	/************************************************************
	 *	@brief		スケール適用後のサイズを返す
	 *	@returns	スケール適用後のサイズ
	 ************************************************************/
	D3DXVECTOR3 GetSizeWithScale();



	/************************************************************
	 *	@brief		バウンディングボックスの計算
	 *	@returns	バウンディングボックス情報
	 ************************************************************/
	MyRect GetBoudingBox2D();



	
	/************************************************************
	 *	@brief		描画処理
	 ************************************************************/
	virtual void Draw();



	/************************************************************
	 *	@brief				作成時に必要な情報を設定
	 *	@param[rect]		切り取り範囲
	 ************************************************************/
	virtual void Load(MyRect rect);


	 
	/************************************************************
	 *	@brief				初期化処理
	 *	@returns			true：成功	false：失敗
	 ************************************************************/
	virtual bool Init(){ return true; }



	 /************************************************************
	  *	@brief				更新処理
	  *	@returns			true：成功	false：失敗
	  ************************************************************/
	 virtual bool Update(float delta);


	 /***********************************************************
	  *	@brief		オブジェクトの名前を設定する
	  **********************************************************/
	 void SetNodeName(LPCSTR name){ m_nodeName = name; }

public:
	static Node* Create(MyRect rect);    //create関数


//macro-----------------------------------------------------------
public:
	R_SYNTHESIZE(BOOL, is_sort_, IsSort);
	R_SYNTHESIZE(float, m_distance, Distance);
	R_SYNTHESIZE(int, m_opacity, Opacity);
};

#endif //!__NODE__
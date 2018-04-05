/*******************************************************
 *	@file		Transform.h
 *	@brief		Transform Class
 *	@data		2017/12/19
 *	@name		hatakeyama
 ******************************************************/

#ifndef  __TRANSFORM__
#define __TRANSFORM__


//preprocessor---------------------------------------
#include "Commponent.h"
#include <vector>


//prototype----------------------------------------
class Node;


class Transform	: public Component
{
//const------------------------------
public :
	static const D3DXVECTOR3 FORWARD;
	static const D3DXVECTOR3 LEFT;
	static const D3DXVECTOR3 RIGHT;
	static const D3DXVECTOR3 BACK;
	static const D3DXVECTOR3 UP;
	static const D3DXVECTOR3 DOWN;

//variabble--------------------------
public:
	
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_anchorPoint;

	D3DXMATRIX m_transMat;
	D3DXMATRIX m_rotateMat;
	D3DXMATRIX m_scaleMat;
	D3DXMATRIX m_btRotateMat;

	Transform* m_parent;

	std::vector<Transform*> m_children;



public:

	Transform();

	~Transform();


	/********************************************************
	 *	@brief		インスタンス生成
	 ********************************************************/
	static Transform* Create(Node* attachTarget);



	/********************************************************
	 *	@brief		初期化する
	 ********************************************************/
	void Init(Node* attachTarget);


	/*******************************************************
	 *	@brief	親の設定
	 *******************************************************/
	void SetParent(Transform* setChild);



	/*******************************************************
	 *	@brief	子Objectの削除
	 *******************************************************/
	void RemoveChild(Transform* child);


	/******************************************************
	 *	@brief	親に削除申請をだして削除してもらう
	 ******************************************************/
	void RemoveFromParent();


	/*******************************************************
	 *	@brief	指定した方向に指定した速さ
	 *******************************************************/
	void Translate(D3DXVECTOR3 moveDir, float speed);


	/*******************************************************
	 *	@brief	指定した方向に回転する
	 *******************************************************/
	void Rotate(D3DXVECTOR3 rotateDir, float speed);
	
};


#endif //!__TRANSFORM__


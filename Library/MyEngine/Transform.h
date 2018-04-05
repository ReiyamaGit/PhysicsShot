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
	 *	@brief		�C���X�^���X����
	 ********************************************************/
	static Transform* Create(Node* attachTarget);



	/********************************************************
	 *	@brief		����������
	 ********************************************************/
	void Init(Node* attachTarget);


	/*******************************************************
	 *	@brief	�e�̐ݒ�
	 *******************************************************/
	void SetParent(Transform* setChild);



	/*******************************************************
	 *	@brief	�qObject�̍폜
	 *******************************************************/
	void RemoveChild(Transform* child);


	/******************************************************
	 *	@brief	�e�ɍ폜�\���������č폜���Ă��炤
	 ******************************************************/
	void RemoveFromParent();


	/*******************************************************
	 *	@brief	�w�肵�������Ɏw�肵������
	 *******************************************************/
	void Translate(D3DXVECTOR3 moveDir, float speed);


	/*******************************************************
	 *	@brief	�w�肵�������ɉ�]����
	 *******************************************************/
	void Rotate(D3DXVECTOR3 rotateDir, float speed);
	
};


#endif //!__TRANSFORM__


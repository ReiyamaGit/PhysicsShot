/************************************************************
 *	@file		Node.h
 *	@brief		�S�Ă�Object�̊�{�` ��`
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
	 *	@brief		�X�P�[���K�p��̃T�C�Y��Ԃ�
	 *	@returns	�X�P�[���K�p��̃T�C�Y
	 ************************************************************/
	D3DXVECTOR3 GetSizeWithScale();



	/************************************************************
	 *	@brief		�o�E���f�B���O�{�b�N�X�̌v�Z
	 *	@returns	�o�E���f�B���O�{�b�N�X���
	 ************************************************************/
	MyRect GetBoudingBox2D();



	
	/************************************************************
	 *	@brief		�`�揈��
	 ************************************************************/
	virtual void Draw();



	/************************************************************
	 *	@brief				�쐬���ɕK�v�ȏ���ݒ�
	 *	@param[rect]		�؂���͈�
	 ************************************************************/
	virtual void Load(MyRect rect);


	 
	/************************************************************
	 *	@brief				����������
	 *	@returns			true�F����	false�F���s
	 ************************************************************/
	virtual bool Init(){ return true; }



	 /************************************************************
	  *	@brief				�X�V����
	  *	@returns			true�F����	false�F���s
	  ************************************************************/
	 virtual bool Update(float delta);


	 /***********************************************************
	  *	@brief		�I�u�W�F�N�g�̖��O��ݒ肷��
	  **********************************************************/
	 void SetNodeName(LPCSTR name){ m_nodeName = name; }

public:
	static Node* Create(MyRect rect);    //create�֐�


//macro-----------------------------------------------------------
public:
	R_SYNTHESIZE(BOOL, is_sort_, IsSort);
	R_SYNTHESIZE(float, m_distance, Distance);
	R_SYNTHESIZE(int, m_opacity, Opacity);
};

#endif //!__NODE__
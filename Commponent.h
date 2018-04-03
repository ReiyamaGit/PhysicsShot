/*******************************************************
 *	@file		Commponent.h		
 *	@brief		Commponent Class ��`
 *	@data		2017/12/19
 *	@name		hatakeyama
 *******************************************************/

#ifndef __COMPONENT__
#define __COMPONENT__


//preprocessor------------------------------------------
#include "Global.h"

class Node;

class Component
{
//variable---------------------------------------------
private:
public:
	LPCSTR m_typeName;
	Node*  m_attachNode;
	

//function---------------------------------------------
public:

	
	/************************************
	 *	@brief	�R���X�g���N�^
	 ************************************/
	Component();

	
	/************************************
	 *	@brief	�f�X�g���N�^
	 ************************************/
	~Component();



	/************************************
	 *	@brief	������
	 ************************************/
	void Init(Node* attachTarget);

};

#endif //!__COMPONENT__


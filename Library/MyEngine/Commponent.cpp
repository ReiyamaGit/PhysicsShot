/*********************************************
 *	@file		Component.cpp
 *	@brief		Component����
 *	@date	    2017/12/19
 *	@name		hatakeyama
 *********************************************/

#include "Commponent.h"

/*********************************************
 *	@brief		Constructor
 *********************************************/
Component::Component()
:m_typeName("")
, m_attachNode(nullptr)
{
}



/*********************************************
 *	@brief		Destructor
 *********************************************/
Component::~Component()
{
	m_attachNode = nullptr;
	m_typeName = "";
}



/**********************************************
 *	@brief		Commponent��Attach���Ă���Ώ�
 **********************************************/
void Component::Init(Node* attachNode)
{
	m_attachNode = attachNode;
}

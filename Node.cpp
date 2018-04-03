/******************************************************
 *	@file		Node.cpp
 *	@brief		Node ����
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "Node.h"
#include "Scene.h"
#include "Transform.h"



/******************************************************
 *	@brief		�R���X�g���N�^
 ******************************************************/
Node::Node()
: m_isUpdate(true)
, m_isActive(true)
, m_opacity(255)
, m_nodeName("")
, m_rect(MyRect(0,0,0,0))
{
	
	m_transform = Transform::Create(this);
	
}





/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
Node::~Node()
{
	
	R_SAFE_DELETE(m_transform);
}



/******************************************************
 *	@brief		Node�C���X�^���X�쐬
 *	@return		����:Node�C���X�^���X  ���s: nullptr
 ******************************************************/
Node* Node::Create(MyRect rect)
{
	Node* node = new Node();

	if (node)
	{
		return node;
	}

	R_SAFE_DELETE(node);

	return nullptr;
}






/******************************************************
 *	@brief		Node�쐬�ɕK�v�ȏ���ݒ肷��
 ******************************************************/
void Node::Load(MyRect rect)
{
	
	m_rect = rect;
	
}



/******************************************************
 *	@brief		�X�P�[����K�p�����T�C�Y���擾����
 *	@return		�X�P�[���K�p��̃T�C�Y
 ******************************************************/
D3DXVECTOR3 Node::GetSizeWithScale()
{
	return D3DXVECTOR3(m_transform->m_size.x * m_transform->m_scale.x
		, m_transform->m_size.y * m_transform->m_scale.y
		, m_transform->m_size.z * m_transform->m_scale.z);

}



/******************************************************
 *	@brief		�o�E���f�B���O�{�b�N�X���쐬����
 *	@return		�쐬���ꂽ�o�E���f�B���O�{�b�N�X
 ******************************************************/
MyRect Node::GetBoudingBox2D()
{
	MyRect rect;

	rect.width = (m_rect.width - m_rect.left) *  m_transform->m_scale.x ;
	rect.height = (m_rect.height - m_rect.top) * m_transform->m_scale.y;
	rect.left = m_transform->m_position.x - m_transform->m_anchorPoint.x * rect.width + GLOBAL::Global::WINDOW_WIDTH / 2.0f;
	rect.top = m_transform->m_position.y - m_transform->m_anchorPoint.y * rect.height + GLOBAL::Global::WINDOW_HEIGHT / 2.0f;

	return rect;
}


/******************************************************
 *	@brief		�q�m�[�h�̕`����s��
 ******************************************************/
void Node::Draw()
{
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		if (m_transform->m_children[i]->m_attachNode->m_isActive)
		{
			m_transform->m_children[i]->m_attachNode->Draw();
		}
	}
	
}




/******************************************************
 *	@brief		�q�m�[�h�̍X�V�����֐����Ă�
 *	@param		�Ō�̃t���[���̏�������
 *	@remark		ism_update��true�̏ꍇ�̂݊֐����Ă�
 ******************************************************/
bool Node::Update(float delta)
{
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		if (m_transform->m_children[i]->m_attachNode->m_isUpdate)
		{
			m_transform->m_children[i]->m_attachNode->Update(delta);
		}
	}


	return true;

}





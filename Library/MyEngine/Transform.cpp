#include "Transform.h"
#include "Node.h"


//static variable initialize----------------------------------------
const D3DXVECTOR3 Transform::FORWARD = D3DXVECTOR3(0, 0, 1);
const D3DXVECTOR3 Transform::LEFT    = D3DXVECTOR3(-1, 0, 0);
const D3DXVECTOR3 Transform::RIGHT   = D3DXVECTOR3(1, 0, 0);
const D3DXVECTOR3 Transform::BACK    = D3DXVECTOR3(0, 0, -1);
const D3DXVECTOR3 Transform::UP		 = D3DXVECTOR3(0, 1, 0);
const D3DXVECTOR3 Transform::DOWN	 = D3DXVECTOR3(0, -1, 0);




/*****************************************************************
 *	@brief		�R���X�g���N�^
 *****************************************************************/
Transform::Transform()
: m_size(D3DXVECTOR3(1, 1, 1))
, m_scale(D3DXVECTOR3(1, 1, 1))
, m_position(D3DXVECTOR3(0, 0, 0))
, m_rotation(D3DXVECTOR3(0, 0, 0))
, m_anchorPoint(D3DXVECTOR3(0, 0, 0))
, m_parent(nullptr)
{
	D3DXMatrixIdentity(&m_transMat);
	D3DXMatrixIdentity(&m_rotateMat);
	D3DXMatrixIdentity(&m_scaleMat);
	D3DXMatrixIdentity(&m_btRotateMat);

	m_children.clear();
}



/*****************************************************************
 *	@brief		�f�X�g���N�^
 *****************************************************************/
Transform::~Transform()
{
	for (int i = m_children.size() - 1; i >= 0; i--)
	{
		R_SAFE_DELETE(m_children[i]->m_attachNode);
		m_children.erase(m_children.begin() + i);
	}
}



/*****************************************************************
 *	@brief					�C���X�^���X�̐���
 *	@param[attachTarget]	�R���|�[�l���g���A�^�b�`����^�[�Q�b�g
 *	@returns				�����F�C���X�^���X ���s�Fnullptr
 *****************************************************************/
Transform* Transform::Create(Node* attachTarget)
{
	Transform* ret = new Transform();

	if (ret)
	{
		ret->Init(attachTarget);
		return ret;
	}

	R_SAFE_DELETE(ret);

	return nullptr;
}





/******************************************************************
 *	@brief					������
 *	@param[attachTarget]	�R���|�[�l���g���A�^�b�`����^�[�Q�b�g
 ******************************************************************/
void Transform::Init(Node* attachTarget)
{
	m_attachNode = attachTarget;
	m_attachNode->AutoRelease();    ///������AutoRelease�̐ݒ��ǉ�����̂͂��������̂�,���ƂŒ���
	m_typeName = "Transform";
}






/******************************************************************
 *	@brief					�e���Z�b�g����
 *	@param[parent]			�Z�b�g�������e
 ******************************************************************/
void Transform::SetParent(Transform* parent)
{
	m_parent = parent;
	parent->m_children.push_back(this);
	m_attachNode->CountRetain();

}




/******************************************************************
 *	@brief					�����̎q�m�[�h���폜����
 *	@param[parent]			�폜�������q�m�[�h
 ******************************************************************/
void Transform::RemoveChild(Transform* child)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
		{
			R_SAFE_DELETE(m_children[i]);

			m_children[i]->m_attachNode->CountRelease();

			m_children.erase(m_children.begin() + i);
		}
	}
}





/******************************************************************
 *	@brief					�e�Ɏ������g���폜�\��������
 ******************************************************************/
void Transform::RemoveFromParent()
{
	m_parent->RemoveChild(this);
}





/******************************************************************
 *	@brief					�w������Ɉړ�����
 *	@param[rotateDir]		�ړ�����
 *	@param[speed]			�ړ��X�s�[�h
 ******************************************************************/

void Transform::Translate(D3DXVECTOR3 moveDir, float speed)
{
	this->m_position += moveDir * speed;
}





/******************************************************************
 *	@brief					�w������ɉ�]������
 *	@param[rotateDir]		��]����
 *	@param[speed]			��]�X�s�[�h
 ******************************************************************/
void Transform::Rotate(D3DXVECTOR3 rotateDir, float speed)
{
	this->m_rotation += rotateDir * speed;
}
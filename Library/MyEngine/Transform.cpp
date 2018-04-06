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
 *	@brief		コンストラクタ
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
 *	@brief		デストラクタ
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
 *	@brief					インスタンスの生成
 *	@param[attachTarget]	コンポーネントをアタッチするターゲット
 *	@returns				成功：インスタンス 失敗：nullptr
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
 *	@brief					初期化
 *	@param[attachTarget]	コンポーネントをアタッチするターゲット
 ******************************************************************/
void Transform::Init(Node* attachTarget)
{
	m_attachNode = attachTarget;
	m_attachNode->AutoRelease();    ///ここでAutoReleaseの設定を追加するのはおかしいので,あとで直す
	m_typeName = "Transform";
}






/******************************************************************
 *	@brief					親をセットする
 *	@param[parent]			セットしたい親
 ******************************************************************/
void Transform::SetParent(Transform* parent)
{
	m_parent = parent;
	parent->m_children.push_back(this);
	m_attachNode->CountRetain();

}




/******************************************************************
 *	@brief					自分の子ノードを削除する
 *	@param[parent]			削除したい子ノード
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
 *	@brief					親に自分自身を削除申請をする
 ******************************************************************/
void Transform::RemoveFromParent()
{
	m_parent->RemoveChild(this);
}





/******************************************************************
 *	@brief					指定方向に移動する
 *	@param[rotateDir]		移動方向
 *	@param[speed]			移動スピード
 ******************************************************************/

void Transform::Translate(D3DXVECTOR3 moveDir, float speed)
{
	this->m_position += moveDir * speed;
}





/******************************************************************
 *	@brief					指定方向に回転をする
 *	@param[rotateDir]		回転方向
 *	@param[speed]			回転スピード
 ******************************************************************/
void Transform::Rotate(D3DXVECTOR3 rotateDir, float speed)
{
	this->m_rotation += rotateDir * speed;
}
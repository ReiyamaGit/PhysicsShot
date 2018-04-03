/******************************************************
 *	@file		Node.cpp
 *	@brief		Node 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "Node.h"
#include "Scene.h"
#include "Transform.h"



/******************************************************
 *	@brief		コンストラクタ
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
 *	@brief		デストラクタ
 ******************************************************/
Node::~Node()
{
	
	R_SAFE_DELETE(m_transform);
}



/******************************************************
 *	@brief		Nodeインスタンス作成
 *	@return		成功:Nodeインスタンス  失敗: nullptr
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
 *	@brief		Node作成に必要な情報を設定する
 ******************************************************/
void Node::Load(MyRect rect)
{
	
	m_rect = rect;
	
}



/******************************************************
 *	@brief		スケールを適用したサイズを取得する
 *	@return		スケール適用後のサイズ
 ******************************************************/
D3DXVECTOR3 Node::GetSizeWithScale()
{
	return D3DXVECTOR3(m_transform->m_size.x * m_transform->m_scale.x
		, m_transform->m_size.y * m_transform->m_scale.y
		, m_transform->m_size.z * m_transform->m_scale.z);

}



/******************************************************
 *	@brief		バウンディングボックスを作成する
 *	@return		作成されたバウンディングボックス
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
 *	@brief		子ノードの描画を行う
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
 *	@brief		子ノードの更新処理関数を呼ぶ
 *	@param		最後のフレームの処理時間
 *	@remark		ism_updateがtrueの場合のみ関数を呼ぶ
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





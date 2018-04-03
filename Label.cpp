/******************************************************
 *	@file		Label.cpp
 *	@brief		Label 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "Label.h"

#include <stdlib.h>



/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Label::Label()
{
	_pFont = nullptr;
	_fontStyle = nullptr;
	
	for (int i = 0; i < 256; i++)
	{
		string_[i] = '\0';
	}
	
}



/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Label::~Label()
{
	R_SAFE_RELEASE(_pFont);
}



/******************************************************
 *	@brief		Labelインスタンス作成
 *	@param		str			文字列
 *	@param		fontstyle	字体
 *	@param		size		サイズ
 *	@return		成功:Labelインスタンス  失敗: nullptr
 ******************************************************/
Label* Label::Create(LPCSTR str,LPCSTR fontstyle,int size)
{
	Label* label = new Label();

	if (label)
	{
		label->Load(fontstyle,size);
		label->SetString(str);

		return label;
	}

	R_SAFE_DELETE(label);

	return nullptr;
}



/******************************************************
 *	@brief		文字の作成情報を設定する
 *	@return		成功:Labelインスタンス	失敗:nullptr
 ******************************************************/
void Label::Load(LPCSTR fontstyle,int size)
{
	m_nodeName = "Label";
	if (FAILED(D3DXCreateFont(GLOBAL::Global::m_device, size, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, fontstyle, &_pFont)))
	{
		R_ASSERT(_pFont);
	}
	
}



/******************************************************
 *	@brief		文字の描画関数
 ******************************************************/
void Label::Draw()
{

	
	
RECT rect;
rect.left = m_transform->m_position.x - m_transform->m_anchorPoint.x * m_transform->m_size.x + GLOBAL::Global::WINDOW_WIDTH / 2.0f;
	rect.top = m_transform->m_position.y - m_transform->m_anchorPoint.y * m_transform->m_size.y + GLOBAL::Global::WINDOW_HEIGHT / 2.0f;
	rect.right = rect.left + m_transform->m_size.x ;
	rect.bottom = rect.top + m_transform->m_size.y ;

	
	
	_pFont->DrawText(NULL, string_, -1, &rect,
		DT_LEFT|DT_TOP, D3DCOLOR_ARGB(m_opacity, 255, 255, 255));

	
}



/******************************************************
 *	@brief		文字列の再描画をする
 *	@param		変換後の文字列
 ******************************************************/
void Label::SetString(LPCSTR str)
{
	strcpy_s(string_, str);

	RECT rect = { 0, 0, 0, 0 };

	_pFont->DrawText(NULL, string_, -1, &rect, DT_CALCRECT, NULL);

	m_transform->m_size.x = rect.right;
	m_transform->m_size.y = rect.bottom;

	Draw();


}





/******************************************************
 *	@file		Label.h
 *	@brief		Labelの定義
 *	@data		2017/06/22
 *	@name		hatakeyama
 ******************************************************/


#ifndef __LABEL__
#define __LABEL__


//preprocessor---------------------------------------
#include "Global.h"
#include "Node.h"


class Label:public Node
{

//variable------------------------------------------
private:

	//D3DXFONTオブジェクトのポインタ
	LPD3DXFONT _pFont;

	//const char*
	//文字列
	TCHAR string_[256];

	//const char*
	//フォントスタイル
	LPCSTR _fontStyle;

	
	
//function------------------------------------------
public:
	Label();
	~Label();

	//文字作成の準備・必要な情報の整理
	void Load(LPCSTR fontstyle,int size);    

	//表示・描画
	void Draw();	
	
	//アンカーポイントを決定する関数
	int Anchor_DeccidedX();
	int Anchor_DeccidedY();

	//再描画
	void SetString(LPCSTR str);

public:
	//指定できること
	//文字列,字体,サイズ
	static Label* Create(LPCSTR str, LPCSTR fontstyle,int size);
};

#endif //!__LABEL__
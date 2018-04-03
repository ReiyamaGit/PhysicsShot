/******************************************************
 *	@file		Label.h
 *	@brief		Label�̒�`
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

	//D3DXFONT�I�u�W�F�N�g�̃|�C���^
	LPD3DXFONT _pFont;

	//const char*
	//������
	TCHAR string_[256];

	//const char*
	//�t�H���g�X�^�C��
	LPCSTR _fontStyle;

	
	
//function------------------------------------------
public:
	Label();
	~Label();

	//�����쐬�̏����E�K�v�ȏ��̐���
	void Load(LPCSTR fontstyle,int size);    

	//�\���E�`��
	void Draw();	
	
	//�A���J�[�|�C���g�����肷��֐�
	int Anchor_DeccidedX();
	int Anchor_DeccidedY();

	//�ĕ`��
	void SetString(LPCSTR str);

public:
	//�w��ł��邱��
	//������,����,�T�C�Y
	static Label* Create(LPCSTR str, LPCSTR fontstyle,int size);
};

#endif //!__LABEL__

/******************************************************
 *	@file		ColorLine.h
 *	@brief		�F��ύX�ł�����̃N���X
 *	@data		2018/03/05
 *	@name		hatakeyama
 ******************************************************/

#ifndef __COLORLINE__
#define __COLORLINE__


//preprocessor------------------------------------
#include "Node.h"


class ColorLine : public Node
{
public:
	enum Color
	{
		DEFAULT = 0xff000000,
		RED		= 0xffff0000,
		BLUE	= 0xff0000ff,
		WHITE	= 0xffffffff,
		GREEN	= 0xff00ff00,
	};

	struct LineVertex
	{
		D3DXVECTOR3 position;
		DWORD color;
	};


private:
	Color m_curColor;
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;
	

public:
	ColorLine();

	~ColorLine();
	


	/************************************************************
	 *	@brief		�`�揈��
	 ************************************************************/
	void Draw() override;



	/************************************************************
	 *	@brief		���݂̐F���擾
	 ************************************************************/
	Color GetCurColor() { return m_curColor; }



	/************************************************************
	 *	@brief		�F�̕ύX
	 ************************************************************/
	void SetCurColor(Color color) { m_curColor = color; }


	/************************************************************
	 *	@brief		����`�悷�鏀��
	 ************************************************************/
	void Init(D3DXVECTOR3 from, D3DXVECTOR3 to, DWORD color);


	/************************************************************
	 *	@brief		ColorLine�C���X�^���X����
	 ************************************************************/
	static ColorLine* Create(D3DXVECTOR3 from, D3DXVECTOR3 to, DWORD color = 0xffffffff);
};

#endif //!__COLORLINE__
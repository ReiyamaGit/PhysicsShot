/************************************************************
 *	@file		ColorLine.cpp
 *	@brief		��(�F�ύX��) �̎���
 *	@date		2018/03/21
 *	@name		hatakeyama
 ************************************************************/


//preprocessor-----------------------------------------------
#include "ColorLine.h"


/************************************************************
 *	@brief		�R���X�g���N�^
 ************************************************************/
ColorLine::ColorLine()
{
}




/************************************************************
 *	@brief		�f�X�g���N�^
 ************************************************************/
ColorLine::~ColorLine()
{
}



/************************************************************
 *	@brief		ColorLine�C���X�^���X����
 ************************************************************/
ColorLine* ColorLine::Create(D3DXVECTOR3 from, D3DXVECTOR3 to, DWORD color)
{
	ColorLine* ret = new ColorLine();

	if (ret)
	{
		ret->Init(from, to, color);
		ret->AutoRelease();
		return ret;
	}

	R_SAFE_DELETE(ret);


	return nullptr;
}



/************************************************************
 *	@brief		����`�悷�鏀��
 ************************************************************/
void ColorLine::Init(D3DXVECTOR3 from, D3DXVECTOR3 to, DWORD color)
{
	
	LineVertex* v;

	m_vertexBuffer->Lock(0, 0, (VOID**)&v, NULL);
	v[0].position = from;
	v[0].color = color;
	v[1].position = to;
	v[1].color = color;

	m_vertexBuffer->Unlock();

}




/************************************************************
 *	@brief		���̕`��
 ************************************************************/
void ColorLine::Draw()
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	GLOBAL::Global::m_device->SetStreamSource(0, m_vertexBuffer, 0, sizeof(LineVertex));
	GLOBAL::Global::m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	GLOBAL::Global::m_device->SetRenderState(D3DRS_LIGHTING, FALSE);

	GLOBAL::Global::m_device->SetTexture(0, NULL);
	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &identity);
	HRESULT h = GLOBAL::Global::m_device->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	GLOBAL::Global::m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

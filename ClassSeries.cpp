/******************************************************
 *	@file		ClassSeries.cpp
 *	@brief		�v�Z�ȂǂɎg�����肷��N���X���܂Ƃ߂�cpp
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/


//MyLibraryPreproccesor-------------------------------
#include "ClassSeries.h"




/********************************************
 *	@brief		�R���X�g���N�^
 ********************************************/
MyRect::MyRect()
{
	left = 0;
	top = 0;
	width = 0;
	height = 0;
}




/*********************************************************
 *	@brief		�Փ˔��������(��`���m)
 *	@param		rectB	�Փ˂����I�u�W�F�N�g
 *	@return		true:�Փ�	false:�Փ˂��Ă��Ȃ�
 *	@remarks	AABB�g�p
 *********************************************************/
bool MyRect::IntersectsRect(MyRect rectB)
{

	float right_first = width;
	float right_second = rectB.width;
	float bottom_first = height;
	float bottom_second = rectB.height;


	if (left > right_second || top > bottom_second || right_first < rectB.left || bottom_first < rectB.top)
	{
		return false;
	}
	else
	{
		return true;
	}


}



/*********************************************************
 *	@brief		�Փ˔��������(��`�Ɠ_)
 *	@param		v	�Փ˂����_
 *	@return		true:�Փ�	false:�Փ˂��Ă��Ȃ�
 *	@remarks	AABB�g�p
 *********************************************************/
bool MyRect::ContainsPoint(D3DXVECTOR2 v)
{
	//�ϐ����킩�肭���̂ł킩��₷����ɓ���܂����B
	float right = width + left;
	float bottom = height + top;

	if (left > v.x || top > v.y || v.x > right || v.y > bottom)
	{
		return false;
	}

	else
	{
		return true;
	}
}


/*************************************************************************
 *	@brief		�X�N���[�����W���烏�[���h���W�ϊ����s��
 *	@param		out			:	���ʂ̏o�͐�ϐ�
 *	@param		mouseX		:	�}�E�X�̃N���b�N�����N���C�A���g���WX
 *	@param		mouseY		:	�}�E�X�̃N���b�N�����N���C�A���g���WY
 *	@param		projZ		:	�ˉe��Ԃł�Z�l
 *	@param		Screen_w	:	�X�N���[���̉���(�E�C���h�E�T�C�Y)
 *	@param		Screen_h	:	�X�N���[���̍���(�E�C���h�E�T�C�Y)
 *	@param		view		:	�r���[�s��
 *	@param		proj		:	�v���W�F�N�V�����s��
 *	@return		D3DXVECTOR3*	�ϊ��ハ�[���h���W
 *************************************************************************/
D3DXVECTOR3* Coordinate::ScreenToWorld(D3DXVECTOR3* out, int mouseX, int mouseY, float projZ, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, view);
	D3DXMatrixInverse(&InvPrj, NULL, proj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(out, &D3DXVECTOR3(mouseX, mouseY, projZ), &tmp);

	return out;
}







/*************************************************************************
*	@brief		���C��XZ���ʂƂ̌�_�̎Z�o
*	@param		out			:	���ʂ̏o�͐�ϐ�
*	@param		mouseX		:	�}�E�X�̃N���b�N�����N���C�A���g���WX
*	@param		mouseY		:	�}�E�X�̃N���b�N�����N���C�A���g���WY
*	@param		projZ		:	�ˉe��Ԃł�Z�l
*	@param		Screen_w	:	�X�N���[���̉���(�E�C���h�E�T�C�Y)
*	@param		Screen_h	:	�X�N���[���̍���(�E�C���h�E�T�C�Y)
*	@param		view		:	�r���[�s��
*	@param		proj		:	�v���W�F�N�V�����s��
*	@return		D3DXVECTOR3*	�ϊ��ハ�[���h���W
*************************************************************************/
D3DXVECTOR3* Coordinate::ScreenToXZPlane(D3DXVECTOR3* out, int mouseX, int mouseY, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	ScreenToWorld(&nearpos, mouseX, mouseY, 0.0f, Screen_w, Screen_h, view, proj);
	ScreenToWorld(&farpos, mouseX, mouseY, 1.0f, Screen_w, Screen_h, view, proj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0)
	{
		// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*out = nearpos + (LP0 / Lray)*ray;
	}
	else
	{
		*out = farpos;
		*out = D3DXVECTOR3(out->x, out->y, 5);
	}

	return out;
}

/******************************************************
 *	@file		Camera.cpp
 *	@brief		Camera ����
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/



#include "Camera.h"



/********************************************
 *	@brief		�R���X�g���N�^
 ********************************************/
Camera::Camera()
{
	m_transform->m_position = D3DXVECTOR3(0, 3, -5);
	m_target = D3DXVECTOR3(0, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);

	m_angle = 60;
	m_aspect = (float)GLOBAL::Global::WINDOW_WIDTH / GLOBAL::Global::WINDOW_HEIGHT;
	m_nearClip = 0.3f;
	m_farClip = 1000.0f;

	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&m_proj);
}



/********************************************
*	@brief		�f�X�g���N�^
********************************************/
Camera::~Camera()
{
	
}




/********************************************
 *	@brief		Camera�̍쐬
 *	@remarks	AutoRelease�Ώ�
 ********************************************/
Camera* Camera::Create()
{
	Camera* pCamera = new Camera();
	pCamera->AutoRelease();

	return pCamera;
}



/***************************************************************
 *	@brief			�X�V
 *	@retrun			true:����  false:���s
 *	@remarks		�r���[�s��̍쐬
					�v���W�F�N�V�����̍쐬
					�r���{�[�h�s��쐬(�r���[�s��ɋt�s�����Z����)
 ****************************************************************/
bool Camera::Update()
{
	D3DXMatrixLookAtLH(&m_view, &m_transform->m_position, &m_target, &m_up);

	GLOBAL::Global::m_device->SetTransform(D3DTS_VIEW, &m_view);
	



	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(m_angle), m_aspect, m_nearClip, m_farClip);

	GLOBAL::Global::m_device->SetTransform(D3DTS_PROJECTION, &m_proj);



	D3DXMatrixLookAtLH(&m_billBoard, &D3DXVECTOR3(0, 0, 0), &(m_target - m_transform->m_position), &m_up);


	D3DXMatrixInverse(&m_billBoard, NULL, &m_billBoard);

	return true;
}





/*************************************************************************
 *	@brief		�N���b�N�����N���C�A���g���W����ray�𐶐�����
 *	@param		mousePosX	�}�E�X���WX 
 *	@param		mousePosY	�}�E�X���WY
 *	@return		ray�x�N�g����Ԃ�
 *************************************************************************/
D3DXVECTOR3 Camera::ScreenPointToRay(float mousePosX ,float mousePosY)
{
	Coordinate coordinate;

	D3DXVECTOR3* farPos = new D3DXVECTOR3();
	D3DXVECTOR3* nearPos = new D3DXVECTOR3();


	D3DXVECTOR3  cameraVector;

	coordinate.ScreenToWorld(nearPos, mousePosX, mousePosY, 0, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT, &m_view, &m_proj);
	coordinate.ScreenToWorld(farPos, mousePosX, mousePosY, 1.0, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT, &m_view, &m_proj);

	cameraVector = *farPos - *nearPos;


	R_SAFE_DELETE(farPos);
	R_SAFE_DELETE(nearPos);

	return *(D3DXVec3Normalize(&cameraVector,&cameraVector));

}
/******************************************************
 *	@file		Light.cpp
 *	@brief		Light ����
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "Light.h"



/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
Light::~Light()
{
}




/******************************************************
 *	@brief		Light�C���X�^���X�쐬
 *	@return		Light�C���X�^���X
 ******************************************************/
Light* Light::Create()
{
	D3DLIGHT9 lightState;


	ZeroMemory(&lightState, sizeof(lightState));


	lightState.Type = D3DLIGHT_DIRECTIONAL;


	lightState.Direction = D3DXVECTOR3(1, -1, 1);


	lightState.Diffuse.r = 1.0f;
	lightState.Diffuse.g = 1.0f;
	lightState.Diffuse.b = 1.0f;



	lightState.Ambient.r = 1.0f;
	lightState.Ambient.g = 1.0f;
	lightState.Ambient.b = 1.0f;


	GLOBAL::Global::m_device->SetLight(0, &lightState);


	GLOBAL::Global::m_device->LightEnable(0, TRUE);


	Light* light = new Light;

	return light;
}


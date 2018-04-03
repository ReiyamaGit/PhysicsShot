
/******************************************************
 *	@file		InputInterface.cpp
 *	@brief		InputInterface ����
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preproccesor--------------------------------
#include "InputInterface.h"
#include "Global.h"



/******************************************************
 *	@brief		�R���X�g���N�^
 ******************************************************/
InputInterface::InputInterface()
{
	m_directInput = nullptr;
	m_keyDevice = nullptr;
	m_mouseDevice = nullptr;

	for (int i = 0; i < 256; i++)
	{
		m_keyState[i] = 0;
		m_prevKeyState[i] = 0;
	}
}



/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
InputInterface::~InputInterface()
{

}





/******************************************************
 *	@brief		����������
 *	@param		�E�B���h�E�n���h��
 *	@remarks	InputDevice���쐬 �� �e�ݒ荀�ڂ�ݒ肷��
 ******************************************************/
void InputInterface::Init(HWND hWnd)
{
	HRESULT result;


	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (LPVOID*)&m_directInput, NULL)))
		return;

	m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyDevice, NULL);

	m_keyDevice->SetDataFormat(&c_dfDIKeyboard);

	m_keyDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, NULL);

	m_mouseDevice->SetDataFormat(&c_dfDIMouse2);

	m_mouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

}




/******************************************************
 *	@brief		�X�V����
 *	@remarks	�f�o�C�X�𓮂����ăX�e�[�g���擾
 *				(�R���g���[���[,�}�E�X,�L�[�{�[�h)
 ******************************************************/
void InputInterface::Update()
{
	m_mouseDevice->Acquire();

	m_prevMouseState = m_mouseState;

	m_mouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState);


	m_keyDevice->Acquire();

	memcpy(m_prevKeyState, m_keyState, sizeof(m_keyState));

	m_keyDevice->GetDeviceState(sizeof(m_keyState), &m_keyState);

	XInputGetState(0, &m_controllerState);

	float value = m_controllerState.Gamepad.bRightTrigger;

}




/********************************************************************************************
 *	@brief		�����ꂽ�L�[���o
 *	@param		keyCode		���o����L�[
 *	@return		true: ���o	false:�񌟏o
 *	@remarks	�L�[�{�[�h���͂�GetDeviceState�œ����L�[���͏�Ԃ��i�[���ꂽ�o�b�t�@��
 *				DIK_����Y�����Ƃ����ӏ���8�r�b�g�ڂ�1�Ȃ�΁��L�[�͓��͂���Ă����ԂƂ����d�g��
 *				�z��(_keyState)�̗v�f�ЂƂЂƂ��Ή�����L�[�̏�Ԃ��i�[����悤�ɂł��Ă���B
 ********************************************************************************************/
BOOL InputInterface::IsKeyPressed(int keyCode)
{

	if (m_keyState[keyCode] & 0x80 && !(m_prevKeyState[keyCode] & 0x80))
	{
		return TRUE;
	}


	return FALSE;
}



/******************************************************
 *	@brief		������Ă���L�[���o
 *	@param		keyCode		���o����L�[
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsKeyPress(int keyCode)
{


	if (m_keyState[keyCode] & 0x80 && m_prevKeyState[keyCode] & 0x80)
	{
		return TRUE;
	}


	return FALSE;
}

/******************************************************
 *	@brief		�����ꂽ�L�[�̌��o
 *	@param		keyCode		���o����L�[
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsKeyReleased(int keyCode)
{

	if (!(m_keyState[keyCode] & 0x80) && m_prevKeyState[keyCode] & 0x80)
	{

		return TRUE;
	}


	return FALSE;
}



/******************************************************
 *	@brief		�}�E�X�̂ǂ����N���b�N�����������o����
 *	@param		keyCode		�N���b�N�ʒu
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsMousePush(int buttonCode)
{
	if (m_mouseState.rgbButtons[buttonCode] & 0x80 && !(m_prevMouseState.rgbButtons[buttonCode] & 0x80))
	{

		return TRUE;
	}

	return FALSE;
}


/******************************************************
 *	@brief		�}�E�X�̂ǂ����N���b�N�����������o����
 *	@param		keyCode		�N���b�N�ʒu
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsMouseMove(int buttonCode)
{
	if (m_mouseState.rgbButtons[buttonCode] & 0x80 && m_prevMouseState.rgbButtons[buttonCode] & 0x80)
	{

		return TRUE;
	}

	return FALSE;
}


/******************************************************
 *	@brief		�}�E�X�̂ǂ����N���b�N�����������o����
 *	@param		keyCode		�N���b�N�ʒu
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsMouseRelease(int buttonCode)
{
	if (!(m_mouseState.rgbButtons[buttonCode] & 0x80) && m_prevMouseState.rgbButtons[buttonCode] & 0x80)
	{

		return TRUE;
	}

	return FALSE;
}



/******************************************************
 *	@brief		�R���g���[���[�̓��͂����o����
 *	@return		true: ���o	false:�񌟏o
 ******************************************************/
BOOL InputInterface::IsGamepadPush()
{
	if (m_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		return TRUE;
	}

	return FALSE;
}



/******************************************************
 *	@brief						Drag���o
 *	@param[buttonCode]			���m����ӏ�
 ******************************************************/
BOOL InputInterface::IsPressingMouse(int buttonCode)
{
	if (m_mouseState.rgbButtons[buttonCode] & 0x80)
	{

		return TRUE;
	}

	return FALSE;
}
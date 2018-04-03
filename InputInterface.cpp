
/******************************************************
 *	@file		InputInterface.cpp
 *	@brief		InputInterface 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preproccesor--------------------------------
#include "InputInterface.h"
#include "Global.h"



/******************************************************
 *	@brief		コンストラクタ
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
 *	@brief		デストラクタ
 ******************************************************/
InputInterface::~InputInterface()
{

}





/******************************************************
 *	@brief		初期化する
 *	@param		ウィンドウハンドル
 *	@remarks	InputDeviceを作成 → 各設定項目を設定する
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
 *	@brief		更新処理
 *	@remarks	デバイスを動かしてステートを取得
 *				(コントローラー,マウス,キーボード)
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
 *	@brief		押されたキー検出
 *	@param		keyCode		検出するキー
 *	@return		true: 検出	false:非検出
 *	@remarks	キーボード入力はGetDeviceStateで得たキー入力状態が格納されたバッファで
 *				DIK_○を添え字とした箇所の8ビット目が1ならば○キーは入力されている状態という仕組み
 *				配列(_keyState)の要素ひとつひとつが対応するキーの状態を格納するようにできている。
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
 *	@brief		押されているキー検出
 *	@param		keyCode		検出するキー
 *	@return		true: 検出	false:非検出
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
 *	@brief		離されたキーの検出
 *	@param		keyCode		検出するキー
 *	@return		true: 検出	false:非検出
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
 *	@brief		マウスのどこをクリックしたかを検出する
 *	@param		keyCode		クリック位置
 *	@return		true: 検出	false:非検出
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
 *	@brief		マウスのどこをクリックしたかを検出する
 *	@param		keyCode		クリック位置
 *	@return		true: 検出	false:非検出
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
 *	@brief		マウスのどこをクリックしたかを検出する
 *	@param		keyCode		クリック位置
 *	@return		true: 検出	false:非検出
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
 *	@brief		コントローラーの入力を検出する
 *	@return		true: 検出	false:非検出
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
 *	@brief						Drag検出
 *	@param[buttonCode]			検知する箇所
 ******************************************************/
BOOL InputInterface::IsPressingMouse(int buttonCode)
{
	if (m_mouseState.rgbButtons[buttonCode] & 0x80)
	{

		return TRUE;
	}

	return FALSE;
}
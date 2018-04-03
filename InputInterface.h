/******************************************************
 *	@file		InputInterface.h
 *	@brief		InputInterface 定義
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/



#ifndef __INPUTINTERFACE__
#define __INPUTINTERFACE__



//preproccesor--------------------------------
#include <dinput.h>
#include "XInput.h"
#include "ClassSeries.h"

#pragma comment(lib,"Xinput.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dInput8.lib")




class InputInterface
{

private:
	BYTE m_keyState[256];							//現在キーの状態

	BYTE m_prevKeyState[256];						//前フレームのキーの状態


	LPDIRECTINPUT8			m_directInput;			//Inputポインタ

	LPDIRECTINPUTDEVICE8	m_keyDevice;			//キーデバイスポインタ


	LPDIRECTINPUTDEVICE8	m_mouseDevice;			//マウスデバイスポインタ

	DIMOUSESTATE2			m_mouseState;			//マウスの入力状態

	DIMOUSESTATE2			m_prevMouseState;		//前回のマウスの入力状態

	XINPUT_STATE			m_controllerState;		//コントローラーの入力状態

	D3DXVECTOR2				m_mouse_pos;			//マウスのポジション(クライアント座標)

	

public:
	InputInterface();

	//デストラクタ
	~InputInterface();
public:
	//初期化
	void Init(HWND hWnd);

	//更新処理
	void Update();

	//キーを押したとき
	BOOL IsKeyPressed(int keyCode);

	//キーを押している間
	BOOL IsKeyPress(int keyCode);

	//キーを離したとき
	BOOL IsKeyReleased(int keyCode);

	//マウスの状態確認
	BOOL IsMousePush(int buttonCode);


	//Mouseが動いているとき
	BOOL IsMouseMove(int buttonCode);


	//MouseのButtonが上がった時
	BOOL IsMouseRelease(int buttonCode);


	//GamePad入力
	BOOL IsGamepadPush();


	//Drag検知
	BOOL IsPressingMouse(int buttonCode);


	/*****************************************************
	 *	@brief		マウスのポジション設定
	 *****************************************************/
	void SetMousePosition(float x, float y){ m_mouse_pos = D3DXVECTOR2(x, y); }


	/*****************************************************
	 *	@brief		マウスのポジション取得
	 *****************************************************/
	D3DXVECTOR2 GetMousePosition(){ return m_mouse_pos; }

	
};

#endif // !__INPUTINTERFACE__

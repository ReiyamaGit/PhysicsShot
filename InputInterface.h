/******************************************************
 *	@file		InputInterface.h
 *	@brief		InputInterface ��`
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
	BYTE m_keyState[256];							//���݃L�[�̏��

	BYTE m_prevKeyState[256];						//�O�t���[���̃L�[�̏��


	LPDIRECTINPUT8			m_directInput;			//Input�|�C���^

	LPDIRECTINPUTDEVICE8	m_keyDevice;			//�L�[�f�o�C�X�|�C���^


	LPDIRECTINPUTDEVICE8	m_mouseDevice;			//�}�E�X�f�o�C�X�|�C���^

	DIMOUSESTATE2			m_mouseState;			//�}�E�X�̓��͏��

	DIMOUSESTATE2			m_prevMouseState;		//�O��̃}�E�X�̓��͏��

	XINPUT_STATE			m_controllerState;		//�R���g���[���[�̓��͏��

	D3DXVECTOR2				m_mouse_pos;			//�}�E�X�̃|�W�V����(�N���C�A���g���W)

	

public:
	InputInterface();

	//�f�X�g���N�^
	~InputInterface();
public:
	//������
	void Init(HWND hWnd);

	//�X�V����
	void Update();

	//�L�[���������Ƃ�
	BOOL IsKeyPressed(int keyCode);

	//�L�[�������Ă����
	BOOL IsKeyPress(int keyCode);

	//�L�[�𗣂����Ƃ�
	BOOL IsKeyReleased(int keyCode);

	//�}�E�X�̏�Ԋm�F
	BOOL IsMousePush(int buttonCode);


	//Mouse�������Ă���Ƃ�
	BOOL IsMouseMove(int buttonCode);


	//Mouse��Button���オ������
	BOOL IsMouseRelease(int buttonCode);


	//GamePad����
	BOOL IsGamepadPush();


	//Drag���m
	BOOL IsPressingMouse(int buttonCode);


	/*****************************************************
	 *	@brief		�}�E�X�̃|�W�V�����ݒ�
	 *****************************************************/
	void SetMousePosition(float x, float y){ m_mouse_pos = D3DXVECTOR2(x, y); }


	/*****************************************************
	 *	@brief		�}�E�X�̃|�W�V�����擾
	 *****************************************************/
	D3DXVECTOR2 GetMousePosition(){ return m_mouse_pos; }

	
};

#endif // !__INPUTINTERFACE__






#include "Global.h"
#include "Game.h"
#include "PoolManager.h"
#include "Scheduler.h"
#include "PlayScene.h"



//#pragma comment(lib,"DirectXLibrary")  //�I���W�i���̃��C�u����

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//�萔�錾
const TCHAR szClassName[] = TEXT("SampleGame");


//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wPara, LPARAM lParam);
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow,HWND &hWnd);
void WindowCreateProcess(HWND hWnd);
void GameProcess();


//Direct3D�I�u�W�F�N�g
LPDIRECT3D9 pD3d;
Game* pGame; //�Q�[���I�u�W�F�N�g



/*-----------------------------------
//WinMain�֐�
//��1�����F�C���X�^���X�n���h��
//��2�����F�C���X�^���X�n���h��
//��3�����F�悭�킩��񂪕����� char *
//��4�����F�Ȃ񂩂悭�킩��񂪐����l
//�߂�l�F�O
/-----------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{

#if _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(53499);
#endif


	HWND hWnd;



	if (!InitApp(hInstance))
	{
		return FALSE;
	}

	if (!InitInstance(hInstance, nCmdShow,hWnd))
	{
		return FALSE;
	}


	

	//���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
	
		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			
			GameProcess();

		}
	}

	//DirectX�J������
	delete pGame;

	return 0;
}


/*-----------------------------------
//�E�C���h�E�v���[�V�[�W��
//��1�����F�E�B���h�E�n���h��
//��2�����F���b�Z�[�W
//��2�����F�����b�Z�[�W
//��2�����F�����b�Z�[�W
//�߂�l�F�f�t�H���g�̃E�C���h�E�v���V�[�W���܂��͂O
/-----------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	int re = 0;
	switch (msg)
	{
	case WM_CREATE:
		WindowCreateProcess(hWnd);
		break;

	//�E�B���h�E���j�󂳂ꂽ�Ƃ�
	case WM_DESTROY:
		PoolManager::GetInstance()->Destroy();
		R_SAFE_DELETE(GLOBAL::Global::m_scheduler);
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		char s[16];
		wsprintf(s, "%d,%d", LOWORD(lParam), HIWORD(lParam));
		SetWindowText(hWnd, s);
		GLOBAL::Global::m_input->SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




ATOM InitApp(HINSTANCE hInst)
{
	//�E�C���h�E�N���X(�݌v�})���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInst;
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);


	return RegisterClassEx(&wc);
}


BOOL InitInstance(HINSTANCE hInst, int nCmdShow,HWND &hWnd)
{

	//�E�C���h�E�T�C�Y���v�Z
	RECT winRect = { 0, 0, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;
	int winH = winRect.bottom - winRect.top;


	//�E�C���h�E���쐬
	hWnd = CreateWindow(szClassName, TEXT("PhysicsShot"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, winW, winH, NULL, NULL, hInst, NULL);

	if (!hWnd)
	{
		return FALSE;
	}


	//�E�C���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);


	return TRUE;
}



/********************************************
 *	@brief		�I�u�W�F�N�g�̍쐬
 *	@param		�E�C���h�E�n���h��
 *	@remarks	�����ň�ԍŏ��̃V�[�������߂Ă���
 ********************************************/
void WindowCreateProcess(HWND hWnd)
{
	pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	
	pGame = new Game();

	GLOBAL::Global::m_scheduler = new Scheduler();

	GLOBAL::Global::m_scheduler->CountRetain();

	pGame->Init(hWnd);

	GLOBAL::Global::m_mainScene = PlayScene::CreateWithPhysicsWorld();

	pGame->InitCheck();
}




/******************************************************************
 *	@brief		�I�u�W�F�N�g�̍쐬
 *	@param		�E�C���h�E�n���h��
 *	@remarks	�֐����Ăԏ��Ԃ͕ς��Ȃ��ł�������
 *				<flow>
 *					���� �� �X�V �� �`�� �� �Q�ƃJ�E���^�`�F�b�N �� �҂�
 ******************************************************************/
void GameProcess()
{
	
	pGame->Input();

	GLOBAL::Global::m_scheduler->Update();

	pGame->Update(GLOBAL::Global::m_scheduler->GetDeltaTime());

	pGame->Draw();

	PoolManager::GetInstance()->Check();

	GLOBAL::Global::m_scheduler->Wait();
}
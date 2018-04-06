




#include "Global.h"
#include "Game.h"
#include "PoolManager.h"
#include "Scheduler.h"
#include "PlayScene.h"



//#pragma comment(lib,"DirectXLibrary")  //オリジナルのライブラリ

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//定数宣言
const TCHAR szClassName[] = TEXT("SampleGame");


//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wPara, LPARAM lParam);
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst, int nCmdShow,HWND &hWnd);
void WindowCreateProcess(HWND hWnd);
void GameProcess();


//Direct3Dオブジェクト
LPDIRECT3D9 pD3d;
Game* pGame; //ゲームオブジェクト



/*-----------------------------------
//WinMain関数
//第1引数：インスタンスハンドル
//第2引数：インスタンスハンドル
//第3引数：よくわからんが文字列 char *
//第4引数：なんかよくわからんが整数値
//戻り値：０
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


	

	//メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
	
		//メッセージあり
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

	//DirectX開放処理
	delete pGame;

	return 0;
}


/*-----------------------------------
//ウインドウプローシージャ
//第1引数：ウィンドウハンドル
//第2引数：メッセージ
//第2引数：副メッセージ
//第2引数：副メッセージ
//戻り値：デフォルトのウインドウプロシージャまたは０
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

	//ウィンドウが破壊されたとき
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
	//ウインドウクラス(設計図)を作成
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

	//ウインドウサイズを計算
	RECT winRect = { 0, 0, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;
	int winH = winRect.bottom - winRect.top;


	//ウインドウを作成
	hWnd = CreateWindow(szClassName, TEXT("PhysicsShot"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, winW, winH, NULL, NULL, hInst, NULL);

	if (!hWnd)
	{
		return FALSE;
	}


	//ウインドウを表示
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);


	return TRUE;
}



/********************************************
 *	@brief		オブジェクトの作成
 *	@param		ウインドウハンドル
 *	@remarks	ここで一番最初のシーンを決めている
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
 *	@brief		オブジェクトの作成
 *	@param		ウインドウハンドル
 *	@remarks	関数を呼ぶ順番は変えないでください
 *				<flow>
 *					入力 → 更新 → 描画 → 参照カウンタチェック → 待ち
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
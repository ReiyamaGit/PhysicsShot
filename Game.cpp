/******************************************************
 *	@file		Game.cpp
 *	@brief		Game 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preproccesor--------------------------------
#include "Game.h"
#include "Scene.h"
#include "InputInterface.h"
#include "EventDispatcher.h"
#include "Material.h"



/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Game::Game()
{
	_pD3d = nullptr;
	GLOBAL::Global::m_device = nullptr;

}



/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Game::~Game()
{
	GLOBAL::Global::m_materialPool->Destroy();
	R_SAFE_RELEASE(GLOBAL::Global::m_device);
	R_SAFE_RELEASE(_pD3d);
	R_SAFE_DELETE(GLOBAL::Global::m_input);
	R_SAFE_DELETE(GLOBAL::Global::m_mainScene);
	R_SAFE_DELETE(GLOBAL::Global::m_scheduler);
	R_SAFE_DELETE(GLOBAL::Global::m_materialPool);
	R_SAFE_DELETE(GLOBAL::Global::m_eventDispatcher);



}



/******************************************************************************************
 *	@brief				初期化する
 *	@param[hWnd]		ウィンドウハンドル
 *	@remarks			描画に必要な設定,EventDispatcherの生成,入力規格,MaterialPoolを作成する
 ******************************************************************************************/
void Game::Init(HWND hWnd)
{
	_pD3d = Direct3DCreate9(D3D_SDK_VERSION);



	D3DPRESENT_PARAMETERS d3dpp;											//プレゼンテーションパラメータ作成 
	ZeroMemory(&d3dpp, sizeof(d3dpp));										//ゼロクリア 引数はクリア対象のアドレスとサイズ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;								//バックバッファのフォーマット
	d3dpp.BackBufferCount = 1;												//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//スワップ動作の設定
	d3dpp.Windowed = TRUE;													//アプリケーションのモード windowならTRUE
	d3dpp.EnableAutoDepthStencil = TRUE;									//
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferWidth = GLOBAL::Global::WINDOW_WIDTH;
	d3dpp.BackBufferHeight = GLOBAL::Global::WINDOW_HEIGHT;
	_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &GLOBAL::Global::m_device);


	
	GLOBAL::Global::m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//アルファブレンドの有効設定
	GLOBAL::Global::m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//これとこの下の1行が半透明部分の表示の仕方
	GLOBAL::Global::m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



	//-----入力規格の作成・初期化-----
	GLOBAL::Global::m_input = new InputInterface();

	GLOBAL::Global::m_input->Init(hWnd);

	//-----EventDispatcher作成-----
	GLOBAL::Global::m_eventDispatcher = EventDispatcher::Create();

	//-----MaterialPool作成-----
	GLOBAL::Global::m_materialPool = new ObjectPool<Material>();

}





/******************************************************
 *	@brief		描画処理
 *	@remarks	画面Clear → 描画開始宣言 → MainSceneの描画 
 *				 → Debugの描画 → 描画終了宣言 
 *				 → 次のバッファコンテンツ表示
 ******************************************************/
void Game::Draw()
{
	GLOBAL::Global::m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	GLOBAL::Global::m_device->BeginScene();

	GLOBAL::Global::m_mainScene->Draw();

	//if (!GLOBAL::Global::m_mainScene->GetPhysicsWorld()->GetActive())
		GLOBAL::Global::m_mainScene->GetPhysicsWorld()->GetDynamicsWorld()->debugDrawWorld();

	GLOBAL::Global::m_device->EndScene();

	GLOBAL::Global::m_device->Present(NULL, NULL, NULL, NULL);
}





/********************************************************
 *	@brief		更新処理
 *	@remarks	物理演算　→ MainSceneの更新 → Debug時更新
 ********************************************************/
void Game::Update(float delta)
{
	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->PhysicsUpdate(delta);


	GLOBAL::Global::m_mainScene->Update(delta);
	

	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->DebugPhysicsUpdate();
	
}


/******************************************************
 *	@brief		入力処理
 *	@remarks	入力規格の状態の更新 → 入力時の処理
 ******************************************************/
void Game::Input()
{

	GLOBAL::Global::m_input->Update();


	GLOBAL::Global::m_eventDispatcher->Input();

}




/*********************************************************
 *	@brief			ゲーム開始時,一部設定のチェック
 *	@remarks		剛体の情報が正しいかチェックする。
 *********************************************************/
void Game::InitCheck()
{
	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->CheckPhysicsBody();
}
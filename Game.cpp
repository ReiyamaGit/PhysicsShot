/******************************************************
 *	@file		Game.cpp
 *	@brief		Game ����
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
 *	@brief		�R���X�g���N�^
 ******************************************************/
Game::Game()
{
	_pD3d = nullptr;
	GLOBAL::Global::m_device = nullptr;

}



/******************************************************
 *	@brief		�f�X�g���N�^
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
 *	@brief				����������
 *	@param[hWnd]		�E�B���h�E�n���h��
 *	@remarks			�`��ɕK�v�Ȑݒ�,EventDispatcher�̐���,���͋K�i,MaterialPool���쐬����
 ******************************************************************************************/
void Game::Init(HWND hWnd)
{
	_pD3d = Direct3DCreate9(D3D_SDK_VERSION);



	D3DPRESENT_PARAMETERS d3dpp;											//�v���[���e�[�V�����p�����[�^�쐬 
	ZeroMemory(&d3dpp, sizeof(d3dpp));										//�[���N���A �����̓N���A�Ώۂ̃A�h���X�ƃT�C�Y)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;								//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	d3dpp.BackBufferCount = 1;												//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//�X���b�v����̐ݒ�
	d3dpp.Windowed = TRUE;													//�A�v���P�[�V�����̃��[�h window�Ȃ�TRUE
	d3dpp.EnableAutoDepthStencil = TRUE;									//
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferWidth = GLOBAL::Global::WINDOW_WIDTH;
	d3dpp.BackBufferHeight = GLOBAL::Global::WINDOW_HEIGHT;
	_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &GLOBAL::Global::m_device);


	
	GLOBAL::Global::m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//�A���t�@�u�����h�̗L���ݒ�
	GLOBAL::Global::m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//����Ƃ��̉���1�s�������������̕\���̎d��
	GLOBAL::Global::m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



	//-----���͋K�i�̍쐬�E������-----
	GLOBAL::Global::m_input = new InputInterface();

	GLOBAL::Global::m_input->Init(hWnd);

	//-----EventDispatcher�쐬-----
	GLOBAL::Global::m_eventDispatcher = EventDispatcher::Create();

	//-----MaterialPool�쐬-----
	GLOBAL::Global::m_materialPool = new ObjectPool<Material>();

}





/******************************************************
 *	@brief		�`�揈��
 *	@remarks	���Clear �� �`��J�n�錾 �� MainScene�̕`�� 
 *				 �� Debug�̕`�� �� �`��I���錾 
 *				 �� ���̃o�b�t�@�R���e���c�\��
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
 *	@brief		�X�V����
 *	@remarks	�������Z�@�� MainScene�̍X�V �� Debug���X�V
 ********************************************************/
void Game::Update(float delta)
{
	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->PhysicsUpdate(delta);


	GLOBAL::Global::m_mainScene->Update(delta);
	

	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->DebugPhysicsUpdate();
	
}


/******************************************************
 *	@brief		���͏���
 *	@remarks	���͋K�i�̏�Ԃ̍X�V �� ���͎��̏���
 ******************************************************/
void Game::Input()
{

	GLOBAL::Global::m_input->Update();


	GLOBAL::Global::m_eventDispatcher->Input();

}




/*********************************************************
 *	@brief			�Q�[���J�n��,�ꕔ�ݒ�̃`�F�b�N
 *	@remarks		���̂̏�񂪐��������`�F�b�N����B
 *********************************************************/
void Game::InitCheck()
{
	GLOBAL::Global::m_mainScene->GetPhysicsWorld()->CheckPhysicsBody();
}
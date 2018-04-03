/******************************************************
 *	@file		Global.cpp
 *	@brief		Global 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preproccesor--------------------------------
#include "Global.h"
#include "Scene.h"


//namespace------------------------------------------
using namespace GLOBAL;


//static variable initialize-------------------------------------
const int				GLOBAL::Global::WINDOW_WIDTH = 1280;

const int				GLOBAL::Global::WINDOW_HEIGHT = 640;


 Camera*				GLOBAL::Global::m_mainCamera = nullptr;

 Scene*					GLOBAL::Global::m_mainScene = nullptr;

 Scheduler*				GLOBAL::Global::m_scheduler = nullptr;

 InputInterface*		GLOBAL::Global::m_input = nullptr;

 EventDispatcher*		GLOBAL::Global::m_eventDispatcher = nullptr;

 LPDIRECT3DDEVICE9		GLOBAL::Global::m_device = nullptr;

 ObjectPool<Material>*	GLOBAL::Global::m_materialPool = nullptr;



/******************************************************
 *	@brief					次のシーンに変更する
 *	@param[pNextScene]		次に表示するScene
 ******************************************************/
void Global::ReplaceScene(Scene* pNextScene)
{
	
	R_SAFE_DELETE(m_mainScene);

	m_mainScene = pNextScene;

	m_mainScene->Init();
}
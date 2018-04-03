/******************************************************
 *	@file		Global.h
 *	@brief		Global ’è‹`
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/


#ifndef __GLOBAL__
#define __GLOBAL__


//preprocessor----------------------------------------
#include <Windows.h>
#include <assert.h>

#include "Constant.h"
#include "InputInterface.h"
#include "ClassSeries.h" 
#include "EventDispatcher.h"
#include "ObjectPool.h"


//prototype-------------------------------------------
class Scene;
class Scheduler;
class Camera;
class Material;



#ifdef __cplusplus
extern "C"{
#endif

	namespace GLOBAL
	{

		class Global
		{

		//constant------------------------------------------
		public:
			static const int WINDOW_WIDTH ;
			static const int WINDOW_HEIGHT ;


		//variable------------------------------------------
		public:


			static Camera* m_mainCamera;				

			static Scene* m_mainScene;

			static Scheduler* m_scheduler;

			static InputInterface* m_input;

			static LPDIRECT3DDEVICE9 m_device;

			static EventDispatcher* m_eventDispatcher;

			static ObjectPool<Material>* m_materialPool;


		//function------------------------------------------
		public:
			void ReplaceScene(Scene* pNextScene);


		};

	}

#ifdef __cplusplus
}
#endif

#endif // !__GLOBAL__

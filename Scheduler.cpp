/******************************************************
 *	@file		Scheduler.cpp
 *	@brief		Scheduler	実装
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//MyLibraryPreproccesor-------------------------------
#include "Scheduler.h"
#include "Global.h"
#include "Label.h"





/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Scheduler::Scheduler()
{

	m_fps = 0;
	m_count = 0;
	m_deltaTime = 0;
	m_startTime = 0;
	m_prevFrameTime = 0;
	

}





/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Scheduler::~Scheduler()
{
}





/******************************************************
 *	@brief		更新処理
 *	@return		true:成功
 ******************************************************/
bool Scheduler::Update()
{
	
	if (m_count == 0)
	{
		m_startTime = GetTickCount();
		m_prevFrameTime = m_startTime;
		
	}
	

	if (m_count == N)
	{
		DWORD t;
		t = GetTickCount();
		m_fps = 1000.0f / ((t - m_startTime) / (float)N);
		m_deltaTime = 1 / m_fps;
		m_count = 0;
		m_startTime = t;

	}
	

	m_count++;

	return true;
}





/***********************************************************
 *	@brief		毎フレーム実行時間同期処理(一定時間にまで待つ)
 ***********************************************************/
void Scheduler::Wait()
{
	DWORD t;
	t = GetTickCount();

	int tookTime = t - m_startTime;
	int waitTime = m_count * 1000 / FPS - tookTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
}




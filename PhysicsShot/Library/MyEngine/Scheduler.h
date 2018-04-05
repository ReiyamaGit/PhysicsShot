/******************************************************
 *	@file		Scheduler.h
 *	@brief		Scheduler	定義
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/



#ifndef __SCHEDULER__
#define __SCHEDULER__



//preprocessor-------------------------------
#include "Node.h"



//protoType--------------------------------------------
class Label;





class Scheduler : public Node
{



//constract-------------------------------------------
private:
	static const int N = 60;
	static const int FPS = 60;



//variable---------------------------------------------
private:
	DWORD		m_startTime;
	int			m_count;
	float		m_deltaTime;
	float		m_fps;
	float		m_prevFrameTime;
	

//function--------------------------------------------
public:
	Scheduler();
	~Scheduler();


	/****************************************
	 *	@brief		更新処理
	 ****************************************/
	bool Update();



	/****************************************
	 *	@brief		待ち時間
	 ****************************************/
	void Wait();


	/****************************************
	 *	@brief		FPS値取得
	 ****************************************/
	float GetFPS(){ return m_fps; }



	/****************************************
	 *	@brief		最終フレーム時間
	 ****************************************/
	float GetDeltaTime() { return 1.0f / (float)FPS; }
};

#endif	//!__SCHEDULER__


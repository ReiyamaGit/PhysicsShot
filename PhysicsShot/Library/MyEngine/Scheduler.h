/******************************************************
 *	@file		Scheduler.h
 *	@brief		Scheduler	��`
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
	 *	@brief		�X�V����
	 ****************************************/
	bool Update();



	/****************************************
	 *	@brief		�҂�����
	 ****************************************/
	void Wait();


	/****************************************
	 *	@brief		FPS�l�擾
	 ****************************************/
	float GetFPS(){ return m_fps; }



	/****************************************
	 *	@brief		�ŏI�t���[������
	 ****************************************/
	float GetDeltaTime() { return 1.0f / (float)FPS; }
};

#endif	//!__SCHEDULER__


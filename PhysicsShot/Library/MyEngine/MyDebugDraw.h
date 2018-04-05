/************************************************************
 *	@file		MyDebugDraw.cpp
 *	@brief		DebugDraw‚Ì’è‹`
 *	@date		2018/03/21
 *	@name		hatakeyama
 ************************************************************/
#ifndef __MYDEBUGDRAW__
#define __MYDEBUGDRAW__


//preprocessor---------------------------------------
#include "LinearMath\btIDebugDraw.h"
#include "Global.h"



class MyDebugDraw : public btIDebugDraw
{

//struct----------------------------------------------
private:
	struct LineVertex
	{
		btVector3 vector;
		DWORD color;
	};

//variable-------------------------------------------
private:
	int m_debugMode;

public:
	MyDebugDraw();

	~MyDebugDraw();

	virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);

	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) {};

	virtual void reportErrorWarning(const char *warningString) {};

	virtual void draw3dText(const btVector3 &location, const char* textString) {};

	virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; };

	virtual int getDebugMode() const { return m_debugMode; }

};

#endif //!__MYDEBUGDRAW__


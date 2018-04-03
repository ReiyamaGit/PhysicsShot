/******************************************************
 *	@file		Camera.h
 *	@brief		Camera ’è‹`
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#ifndef __CAMERA___
#define __CAMERA___


//preprocessor----------------------------------------------
#include "Node.h"
#include "Constant.h"


/*****************************************
 *	@brief		Camera Class
 *****************************************/
class Camera:public Node
{


//variable--------------------------------
protected:
	float m_angle;
	float m_aspect;
	float m_nearClip;
	float m_farClip;


	D3DXVECTOR3 m_up;


//function--------------------------------
public:
	Camera();
	~Camera();

	bool Update();


	D3DXVECTOR3 ScreenPointToRay(float mousePosX, float mousePosY);

	static Camera* Create();

	


//Macro-----------------------------------
public:

	R_SYNTHESIZE(D3DXMATRIX, m_billBoard, BillBoard);
	R_SYNTHESIZE(D3DXVECTOR3, m_target, Target);
	R_SYNTHESIZE(D3DXMATRIX, m_view, View);
	R_SYNTHESIZE(D3DXMATRIX, m_proj, Projection);

};

#endif	//!__CAMERA__
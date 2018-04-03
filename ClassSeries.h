/******************************************************
 *	@file		ClassSeries.h
 *	@brief		ClassSeries ’è‹`
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#ifndef __CLASSSERIES__
#define __CLASSSERIES__


//preprocessor----------------------------------------
#include <d3dx9.h>



/****************************************************
 *	@brief	Size Class
 ***************************************************/
class Size
{

//variable-----------------------------------
public:
	float x;
	float y;
	float z;

//function-----------------------------------
public:

	Size() {};

	inline Size::Size(float x1, float y1, float z1)
	{
		x = x1;
		y = y1;
		z = z1;
	};

	inline Size::Size(float x1, float y1)
	{
		x = x1;
		y = y1;
		z = 0;
	};

	inline Size::Size(D3DXVECTOR3 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}


};


/****************************************************
 *	@brief	MyRect Class
 ****************************************************/
class MyRect
{

//variable-----------------------------------
public:
	float left;
	float top;
	float width;
	float height;


//function-----------------------------------
public:

	MyRect();


	inline MyRect::MyRect(float dleft, float dtop, float dwidth, float dheight)
	{
		left = dleft;
		top = dtop;
		width = dwidth;
		height = dheight;

	};

	bool IntersectsRect(MyRect rectB);


	bool ContainsPoint(D3DXVECTOR2 v);


};




/**************************
 *	@brief	Coordinate Class
 ***************************/
class Coordinate
{

//function-------------------------------------------------------------
public:

	D3DXVECTOR3* ScreenToWorld(D3DXVECTOR3* out, int sx, int sy, float projZ, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* prj);
	D3DXVECTOR3* ScreenToXZPlane(D3DXVECTOR3* out, int sx, int sy, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* proj);

};


#endif //!__CLASSSERIES__
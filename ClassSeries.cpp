/******************************************************
 *	@file		ClassSeries.cpp
 *	@brief		計算などに使ったりするクラスをまとめたcpp
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/


//MyLibraryPreproccesor-------------------------------
#include "ClassSeries.h"




/********************************************
 *	@brief		コンストラクタ
 ********************************************/
MyRect::MyRect()
{
	left = 0;
	top = 0;
	width = 0;
	height = 0;
}




/*********************************************************
 *	@brief		衝突判定をする(矩形同士)
 *	@param		rectB	衝突したオブジェクト
 *	@return		true:衝突	false:衝突していない
 *	@remarks	AABB使用
 *********************************************************/
bool MyRect::IntersectsRect(MyRect rectB)
{

	float right_first = width;
	float right_second = rectB.width;
	float bottom_first = height;
	float bottom_second = rectB.height;


	if (left > right_second || top > bottom_second || right_first < rectB.left || bottom_first < rectB.top)
	{
		return false;
	}
	else
	{
		return true;
	}


}



/*********************************************************
 *	@brief		衝突判定をする(矩形と点)
 *	@param		v	衝突した点
 *	@return		true:衝突	false:衝突していない
 *	@remarks	AABB使用
 *********************************************************/
bool MyRect::ContainsPoint(D3DXVECTOR2 v)
{
	//変数名わかりくいのでわかりやすいやつに入れました。
	float right = width + left;
	float bottom = height + top;

	if (left > v.x || top > v.y || v.x > right || v.y > bottom)
	{
		return false;
	}

	else
	{
		return true;
	}
}


/*************************************************************************
 *	@brief		スクリーン座標からワールド座標変換を行う
 *	@param		out			:	結果の出力先変数
 *	@param		mouseX		:	マウスのクリックしたクライアント座標X
 *	@param		mouseY		:	マウスのクリックしたクライアント座標Y
 *	@param		projZ		:	射影空間でのZ値
 *	@param		Screen_w	:	スクリーンの横幅(ウインドウサイズ)
 *	@param		Screen_h	:	スクリーンの高さ(ウインドウサイズ)
 *	@param		view		:	ビュー行列
 *	@param		proj		:	プロジェクション行列
 *	@return		D3DXVECTOR3*	変換後ワールド座標
 *************************************************************************/
D3DXVECTOR3* Coordinate::ScreenToWorld(D3DXVECTOR3* out, int mouseX, int mouseY, float projZ, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, view);
	D3DXMatrixInverse(&InvPrj, NULL, proj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(out, &D3DXVECTOR3(mouseX, mouseY, projZ), &tmp);

	return out;
}







/*************************************************************************
*	@brief		レイとXZ平面との交点の算出
*	@param		out			:	結果の出力先変数
*	@param		mouseX		:	マウスのクリックしたクライアント座標X
*	@param		mouseY		:	マウスのクリックしたクライアント座標Y
*	@param		projZ		:	射影空間でのZ値
*	@param		Screen_w	:	スクリーンの横幅(ウインドウサイズ)
*	@param		Screen_h	:	スクリーンの高さ(ウインドウサイズ)
*	@param		view		:	ビュー行列
*	@param		proj		:	プロジェクション行列
*	@return		D3DXVECTOR3*	変換後ワールド座標
*************************************************************************/
D3DXVECTOR3* Coordinate::ScreenToXZPlane(D3DXVECTOR3* out, int mouseX, int mouseY, int Screen_w, int Screen_h, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	ScreenToWorld(&nearpos, mouseX, mouseY, 0.0f, Screen_w, Screen_h, view, proj);
	ScreenToWorld(&farpos, mouseX, mouseY, 1.0f, Screen_w, Screen_h, view, proj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0)
	{
		// 床交点
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*out = nearpos + (LP0 / Lray)*ray;
	}
	else
	{
		*out = farpos;
		*out = D3DXVECTOR3(out->x, out->y, 5);
	}

	return out;
}
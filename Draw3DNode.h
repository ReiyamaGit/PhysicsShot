#ifndef __DRAW3DNODE__
#define __DRAW3DNODE__

#include "Fbx.h"


class Draw3DNode : public Fbx
{
private:
	const FLOAT SLICE;
	const FLOAT STACK;
public:
	enum NodeType
	{
		BOX,
		SPHERE,
	};

	static enum Color3D
	{
		RED,
		BLUE,
		GREEN,
		BLACK,
		WHITE,
	};


private:
	float			m_radius;
	LPD3DXMESH		m_mesh;
	NodeType		m_type;
	D3DMATERIAL9    m_material;
	D3DXVECTOR3		m_boxData;
	Color3D			m_color;


public:
	Draw3DNode();
	~Draw3DNode();

	void CreateSphere(float radius, Color3D color);

	void CreateBox(D3DXVECTOR3 v, Color3D color);

	void Draw() override;

	void CreateMaterial();

	D3DXMATRIX CreateWorldMatrix();

	static Draw3DNode* Create();
};


#endif // __DRAW3DNODE__
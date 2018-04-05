#pragma once
#include "Node.h"

class Camera;

class Quad:public Node
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 _indexBuffer;
	D3DMATERIAL9 _material;
	LPDIRECT3DTEXTURE9 _pTexture;

public:
	Quad();
	~Quad();
	static Quad* Create(LPCSTR fileName);
	void Load(LPCSTR fileName);
	void Draw();

	
};


/******************************************************
 *	@file		Fbx.cpp
 *	@brief		FBX 実装
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/


 //preprocessor--------------------------------
#include "Fbx.h"
#include "Camera.h"
#include "PhysicsBody.h"


/************************************************
 *	@brief		コンストラクタ
 ************************************************/
Fbx::Fbx()
	: m_vertexBuffer(nullptr)
	, m_manager(nullptr)
	, m_importer(nullptr)
	, m_scene(nullptr)
{
}



/************************************************
 *	@brief		デストラクタ
 ************************************************/
Fbx::~Fbx()
{
	R_SAFE_RELEASE(m_vertexBuffer);
	R_SAFE_DELETE(m_physicsBody);

	m_scene->Destroy();
	m_manager->Destroy();

	

	delete[] m_materialData;
}







/************************************************
 *	@brief		FBX作成
 *	@param		fileName	FBXファイル名前
 *	@return		成功：FBX  失敗：nullptr
 ************************************************/
Fbx* Fbx::Create(LPCSTR fileName)
{
	auto fbx = new Fbx();
	fbx->Load(fileName);
	return fbx;
}







/************************************************************************
 *	@brief		FBX作成準備。
 *	@param		fileName	FBXファイル名
 *	@remarks	1.マネージャの作成,それを元にインポーターとシーンを作成
 *				2.インポーターを使用してFBXファイルを開く
 *				3.開いたFBXファイルをシーンに渡し,インポーターを解放する
 *				4.FBXファイルは複数のデータが階層構造のルートをとってくる
 *				5.FBXの子の取得。取得した分だけcheckNodeの関数を呼ぶ
 ************************************************************************/
void Fbx::Load(LPCSTR fileName)
{
	m_nodeName = "FBX";

	m_materialData = Material::Create();

	m_manager = FbxManager::Create();
	m_importer = FbxImporter::Create(m_manager, "");
	m_scene = FbxScene::Create(m_manager, "");


	m_importer->Initialize(fileName);



	m_importer->Import(m_scene);



	m_importer->Destroy();


	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);


	char dir[MAX_PATH];
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);
	SetCurrentDirectory(dir);


	FbxNode* rootNode = m_scene->GetRootNode();

	int childCount = rootNode->GetChildCount();

	for (int i = 0; childCount > i; i++)
	{

		CheckNode(rootNode->GetChild(i));
	}


	SetCurrentDirectory(defaultCurrentDir);




}




/*********************************************************************
 *	@brief		FBXの描画をする
 *	@remarks	頂点情報を渡す
 *				使うFVFをセット(XYZ(移動),NORMAL(法線),TEX1(テクスチャ)
 **********************************************************************/
void Fbx::Draw()
{
	Node::Draw();

	GLOBAL::Global::m_device->SetStreamSource(0, m_vertexBuffer, 0, sizeof(Vertex));

	GLOBAL::Global::m_device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL /*| D3DFVF_TEX1*/);


	D3DXMATRIX world = CreateWorldMatrix();

	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &world);

	//シェーダー使用時
	if (m_materialData->m_shaderEffect)
	{
		m_materialData->onShader();
		m_materialData->m_shaderEffect->Begin(0, 0);
		m_materialData->m_shaderEffect->BeginPass(0);
	}


	for (int i = 0; i < m_materialCount; i++)
	{
		GLOBAL::Global::m_device->SetIndices(m_materialData[i].m_indexBuffer);
		GLOBAL::Global::m_device->SetMaterial(&m_materialData[i].m_material);
		GLOBAL::Global::m_device->SetTexture(0, NULL);
		GLOBAL::Global::m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_materialData[i].m_polgonCountOfMaterial);
	}


	if (m_materialData->m_shaderEffect)
	{

		m_materialData->m_shaderEffect->EndPass();
		m_materialData->m_shaderEffect->End();
	}

}






/**********************************************************************
 *	@brief		引数ノードがメッシュ情報なのかを調べる
 *	@remarks	メッシュ情報の場合,
 *				メッシュ情報から頂点情報とインデックス情報を作成する
 *				メッシュ情報ではない場合,
 *				引数ノードの下の階層を再帰でその下の階層をすべて探索する
 **********************************************************************/
void Fbx::CheckNode(FbxNode* pNode)
{

	FbxNodeAttribute* attr = pNode->GetNodeAttribute();

	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{

		m_materialCount = pNode->GetMaterialCount();
		m_materialData = new Material[m_materialCount];

		for (int i = 0; i < m_materialCount; i++)
		{


			FbxSurfacePhong* phong = (FbxSurfacePhong*)pNode->GetMaterial(i);
			FbxDouble3 diffuse = phong->Diffuse;
			FbxDouble3 ambient = phong->Ambient;
			FbxDouble3 speculer;

			ZeroMemory(&m_materialData[i], sizeof(D3DMATERIAL9));

			m_materialData[i].m_material.Diffuse.r = (float)diffuse[0];
			m_materialData[i].m_material.Diffuse.g = (float)diffuse[1];
			m_materialData[i].m_material.Diffuse.b = (float)diffuse[2];
			m_materialData[i].m_material.Diffuse.a = 1.0f;

			m_materialData[i].m_material.Ambient.r = (float)ambient[0];
			m_materialData[i].m_material.Ambient.g = (float)ambient[1];
			m_materialData[i].m_material.Ambient.b = (float)ambient[2];
			m_materialData[i].m_material.Ambient.a = 1.0f;


			if (pNode->GetMaterial(i)->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				speculer = phong->Specular;
				m_materialData[i].m_material.Specular.r = (FLOAT)speculer[0];
				m_materialData[i].m_material.Specular.g = (FLOAT)speculer[1];
				m_materialData[i].m_material.Specular.b = (FLOAT)speculer[2];

				m_materialData[i].m_material.Power = (float)phong->Shininess;
			}

			FbxProperty property = pNode->GetMaterial(i)->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxFileTexture* texture_file = property.GetSrcObject<FbxFileTexture>(0);

			if (texture_file == NULL)
			{
				m_materialData[i].m_texture = NULL;

			}
			else
			{
				const char* texture_file_name = texture_file->GetFileName();


				char name[_MAX_FNAME];
				char ext[_MAX_EXT];
				_splitpath_s(texture_file_name, NULL, 0, NULL, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				D3DXCreateTextureFromFileEx(GLOBAL::Global::m_device, name, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
					D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &m_materialData[i].m_texture);


			}


		}
		CheckMesh(pNode->GetMesh());
	}
	else
	{
		int childCount = pNode->GetChildCount();

		for (int i = 0; childCount > i; i++)
		{
			CheckNode(pNode->GetChild(i));
		}
	}

}





/********************************************************************************************
 *	@brief		メッシュから情報を得る
 *	@remarks	pMeshのControllPoint・頂点個数・ポリゴン数・インデックス数を取得
 *				ControllPointは配列の先頭で各要素には頂点座標が入っているので,それを移す
 *				インデックスを格納。
 ********************************************************************************************/
void Fbx::CheckMesh(FbxMesh* pMesh)
{


	FbxVector4* pVertexPos = pMesh->GetControlPoints();
	m_vertexCount = pMesh->GetControlPointsCount();
	m_polygonCount = pMesh->GetPolygonCount();
	m_indexCount = pMesh->GetPolygonVertexCount();

	Vertex* vertexList = new Vertex[m_vertexCount];



	for (int i = 0; m_vertexCount > i; i++)
	{
		vertexList[i].pos.x = (float)pVertexPos[i][0];
		vertexList[i].pos.y = (float)pVertexPos[i][1];
		vertexList[i].pos.z = (float)pVertexPos[i][2];
	}

	for (int i = 0; i < m_polygonCount; i++)
	{
		int startIndex = pMesh->GetPolygonVertexIndex(i);


		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];


			FbxVector4 Normal;

			pMesh->GetPolygonVertexNormal(i, j, Normal);


			vertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);


			FbxVector2 uv = pMesh->GetLayer(0)->GetUVs()->GetDirectArray().GetAt(index);
			vertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));

		}
	}

	for (int i = 0; i < m_polygonCount; i++)
	{
		int startIndex = pMesh->GetPolygonVertexIndex(i);

		D3DXVECTOR3 tan = CreateTan(&vertexList[pMesh->GetPolygonVertices()[startIndex + 0]].pos,
			&vertexList[pMesh->GetPolygonVertices()[startIndex + 1]].pos,
			&vertexList[pMesh->GetPolygonVertices()[startIndex + 2]].pos,
			&vertexList[pMesh->GetPolygonVertices()[startIndex + 0]].uv,
			&vertexList[pMesh->GetPolygonVertices()[startIndex + 1]].uv,
			&vertexList[pMesh->GetPolygonVertices()[startIndex + 2]].uv);

		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];
			vertexList[index].tangent = tan;
		}
	}
	GLOBAL::Global::m_device->CreateVertexBuffer(sizeof(Vertex)* m_vertexCount, 0,
		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, D3DPOOL_MANAGED,
		&m_vertexBuffer, 0);


	Vertex* vCopy;
	m_vertexBuffer->Lock(0, 0, (void**)&vCopy, 0);
	memcpy(vCopy, vertexList, sizeof(Vertex)* m_vertexCount);
	m_vertexBuffer->Unlock();

	delete[] vertexList;



	for (int i = 0; i < m_materialCount; i++)
	{
		int* indexList = new int[m_indexCount];

		int count = 0;

		for (int polygon = 0; polygon < m_polygonCount; polygon++)
		{
			int materialID = pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(polygon);

			if (materialID == i)
			{
				for (int vertex = 0; vertex < 3; vertex++)
				{
					indexList[count++] = pMesh->GetPolygonVertex(polygon, vertex);
				}
			}
		}


		m_materialData[i].m_polgonCountOfMaterial = count / 3;

		GLOBAL::Global::m_device->CreateIndexBuffer(sizeof(int)* m_indexCount, 0,
			D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_materialData[i].m_indexBuffer, 0);

		DWORD* iCopy;
		m_materialData[i].m_indexBuffer->Lock(0, 0, (void**)&iCopy, 0);
		memcpy(iCopy, indexList, sizeof(int)* m_indexCount);
		m_materialData[i].m_indexBuffer->Unlock();
		delete[] indexList;


	}


}


/*****************************************************************************
 *	@brief		引数で受け取ったRayが自分に当たるかを判定する
 *	@param		data		Rayのデータ
 *	@remarks	インデックス情報を取得。
 マテリアルのポリゴンごとに頂点を取得する
 取得した頂点にワールド行列を適用する
 レイを飛ばしてあたった場合,引数にHitしたことと飛距離を教えてあげる
 *****************************************************************************/
void Fbx::RayCast(RayCastData *data)
{


	D3DXMATRIX world = CreateWorldMatrix();

	data->hit = FALSE;
	data->dist = 99999.0f;


	Vertex *vCopy;

	m_vertexBuffer->Lock(0, 0, (void**)&vCopy, 0);


	for (DWORD i = 0; i < m_materialCount; i++)
	{
		DWORD *iCopy;
		m_materialData[i].m_indexBuffer->Lock(0, 0, (void**)&iCopy, 0);

		for (DWORD j = 0; j < m_materialData[i].m_polgonCountOfMaterial; j++)
		{
			D3DXVECTOR3 ver[3];
			ver[0] = vCopy[iCopy[j * 3 + 0]].pos;
			ver[1] = vCopy[iCopy[j * 3 + 1]].pos;
			ver[2] = vCopy[iCopy[j * 3 + 2]].pos;


			D3DXVec3TransformCoord(&ver[0], &ver[0], &world);
			D3DXVec3TransformCoord(&ver[1], &ver[1], &world);
			D3DXVec3TransformCoord(&ver[2], &ver[2], &world);


			BOOL  hit;
			float dist;

			hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2],
				&data->start, &data->dir, NULL, NULL, &dist);

			if (hit && data->dist > dist)
			{
				data->hit = TRUE;
				data->dist = dist;
			}

		}

		m_materialData[i].m_indexBuffer->Unlock();
	}

	m_vertexBuffer->Unlock();
}



/****************************************************
 *	@brief		ワールド行列を作成する
 *	@return		ワールド行列
 *	@remarks	移動行列をもとめる
 回転行列を求める
 拡大縮小行列を求める
 最後に,scale,rotate,transの順に乗算
 ****************************************************/
D3DXMATRIX Fbx::CreateWorldMatrix()
{
	//移動行列

	D3DXMatrixTranslation(&m_transform->m_transMat, m_transform->m_parent->m_position.x + m_transform->m_position.x
		, m_transform->m_parent->m_position.y + m_transform->m_position.y
		, m_transform->m_parent->m_position.z + m_transform->m_position.z);

	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;

	//回転行列
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(m_transform->m_parent->m_rotation.x + m_transform->m_rotation.x));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(m_transform->m_parent->m_rotation.y + m_transform->m_rotation.y));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(m_transform->m_parent->m_rotation.z + m_transform->m_rotation.z));

	m_transform->m_rotateMat = matRotateX * matRotateY * matRotateZ;

	//拡大縮小
	D3DXMatrixScaling(&m_transform->m_scaleMat, m_transform->m_scale.z
		, m_transform->m_scale.y
		, m_transform->m_scale.z);



	//ワールド行列
	D3DXMATRIX parentWorld = m_transform->m_parent->m_scaleMat * m_transform->m_parent->m_rotateMat * m_transform->m_parent->m_transMat;

	D3DXMATRIX world = m_transform->m_scaleMat * m_transform->m_rotateMat * m_transform->m_transMat;






	return  world * parentWorld;
}



/************************************************
 *	@brief	タンジェントの計算
 ************************************************/
D3DXVECTOR3 Fbx::CreateTan(D3DXVECTOR3 *pos0, D3DXVECTOR3 *pos1, D3DXVECTOR3 *pos2,
	D3DXVECTOR2 *uv0, D3DXVECTOR2 *uv1, D3DXVECTOR2 *uv2)
{
	D3DXVECTOR3 cp0[3] = {
		D3DXVECTOR3(pos0->x, uv0->x, uv0->y),
		D3DXVECTOR3(pos0->y, uv0->x, uv0->y),
		D3DXVECTOR3(pos0->z, uv0->x, uv0->y)
	};

	D3DXVECTOR3 cp1[3] = {
		D3DXVECTOR3(pos1->x, uv1->x, uv1->y),
		D3DXVECTOR3(pos1->y, uv1->x, uv1->y),
		D3DXVECTOR3(pos1->z, uv1->x, uv1->y)
	};

	D3DXVECTOR3 cp2[3] = {
		D3DXVECTOR3(pos2->x, uv2->x, uv2->y),
		D3DXVECTOR3(pos2->y, uv2->x, uv2->y),
		D3DXVECTOR3(pos2->z, uv2->x, uv2->y)
	};

	float f[3];
	for (int i = 0; i < 3; i++)
	{
		D3DXVECTOR3 v1 = cp1[i] - cp0[i];
		D3DXVECTOR3 v2 = cp2[i] - cp1[i];
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &v1, &v2);

		f[i] = -cross.y / cross.x;
	}

	D3DXVECTOR3 tangent;
	memcpy(tangent, f, sizeof(float) * 3);
	D3DXVec3Normalize(&tangent, &tangent);
	return tangent;
}


void Fbx::SetPhysicsBody(PhysicsBody* body)
{
	m_physicsBody = body;
}
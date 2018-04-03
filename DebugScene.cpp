/***************************************************************
 *	@file				DebugScene.h
 *	@brief				DebugScene Class 定義
 *	@date				2018/01/24
 *	@name				hatakeyama
 ***************************************************************/



 //preprocessor------------------------------------------
#include "DebugScene.h"
#include "Scheduler.h"
#include "ToggleButton.h"
#include "TouchListener.h"
#include "Fbx.h"
#include "Camera.h"
#include "Label.h"
#include "./PhysicsBody.h"


//const-------------------------------------------------
const float TRANSLATE_SPEED = 0.3f;				//移動速度
const float ROTATE_SPEED = 0.8f;				//回転速度
const int FONT_SIZE = 20;
const float BUTTON_TEXTURE_SIZE_H = 64;
const float CAMERA_ZOOM_STD_POINT = 20;
const float CAMERA_SENSITYVITY = 0.01f;


/********************************************************
 *	@brief		コンストラクタ - Constructor
 ********************************************************/
DebugScene::DebugScene()
	: m_selectedItem(nullptr)
	, m_fps(nullptr)
	, m_isEdit(false)
{
	m_debugItemList.clear();
}



/********************************************************
 *	@brief		デストラクタ - Desturctor
 ********************************************************/
DebugScene::~DebugScene()
{
}




/******************************************************
 *	@brief		初期化処理 - Initialize Process
 ******************************************************/
bool DebugScene::Init()
{
	if (!Scene::Init())
	{
		return false;
	}

	//FPS表示
	TCHAR str[20];
	sprintf_s(str, TEXT("FPS = %.1f"), GLOBAL::Global::m_scheduler->GetFPS());
	m_fps = Label::Create(str, "", FONT_SIZE);
	m_fps->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, -GLOBAL::Global::WINDOW_HEIGHT / 2.0f, 0);
	m_fps->m_transform->m_anchorPoint = D3DXVECTOR3(0, 0, 0);
	m_fps->m_transform->SetParent(this->m_transform);
	m_fps->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_fps);

	//Mode切り替えButton
	CreateEditButton();

	//x座標
	sprintf_s(str, TEXT("x = %.1f"), 0);
	m_xPos = Label::Create(str, "", FONT_SIZE);
	m_xPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_editButton->m_transform->m_position.y + BUTTON_TEXTURE_SIZE_H, 0);
	m_xPos->m_transform->SetParent(this->m_transform);
	m_xPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_xPos);

	//y座標
	sprintf_s(str, TEXT("y = %.1f"), 0);
	m_yPos = Label::Create(str, "", FONT_SIZE);
	m_yPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_xPos->m_transform->m_position.y + FONT_SIZE, 0);
	m_yPos->m_transform->SetParent(this->m_transform);
	m_yPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_yPos);


	//z座標
	sprintf_s(str, TEXT("z = %.1f"), 0);
	m_zPos = Label::Create(str, "", FONT_SIZE);
	m_zPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_yPos->m_transform->m_position.y + FONT_SIZE, 0);
	m_zPos->m_transform->SetParent(this->m_transform);
	m_zPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_zPos);

	//DebugScene入力処理
	DebugScene::Input();

	return true;
}


/******************************************************
 *	@brief			更新処理
 *	@param[delta]	最終フレーム実行時間
 ******************************************************/
bool DebugScene::Update(float delta)
{
	//FPSの表示
	if (m_fps)
	{
		TCHAR str[20];
		sprintf_s(str, TEXT("FPS = %.1f"), GLOBAL::Global::m_scheduler->GetFPS());
		m_fps->SetString(str);
	}


	//選択されたItemの座標表示
	if (m_selectedItem)
	{
		TCHAR str[20];
		sprintf_s(str, TEXT("x=%.1f"), m_selectedItem->m_transform->m_position.x);
		m_xPos->SetString(str);
		sprintf_s(str, TEXT("x=%.1f"), m_selectedItem->m_transform->m_position.y);
		m_yPos->SetString(str);
		sprintf_s(str, TEXT("z = %.1f"), m_selectedItem->m_transform->m_position.z);
		m_zPos->SetString(str);
	}


	//DebugMode時
	if (m_isEdit)
	{
		ItemUpDown();

		//Cameraのコントロール部分
		if (GLOBAL::Global::m_input->IsMousePush(1))
		{
			m_touchBeginPos = GLOBAL::Global::m_input->GetMousePosition();
		}

		if (GLOBAL::Global::m_input->IsMouseMove(1))
		{
			D3DXVECTOR2 vec = GLOBAL::Global::m_input->GetMousePosition() - m_touchBeginPos;
			GLOBAL::Global::m_mainCamera->m_transform->m_position = D3DXVECTOR3(cosf(vec.x * CAMERA_SENSITYVITY) * (vec.y + CAMERA_ZOOM_STD_POINT)
																				, GetCamera()->m_transform->m_position.y, sinf(vec.x * CAMERA_SENSITYVITY) * (vec.y + CAMERA_ZOOM_STD_POINT));
			GLOBAL::Global::m_mainCamera->Update();
		}
		if (GLOBAL::Global::m_input->IsMouseRelease(1))
		{
			m_touchBeginPos = GLOBAL::Global::m_input->GetMousePosition();
		}
		
		
		return false;
	}

	

	if (!Scene::Update(delta))
	{
		return false;
	}

	
	return true;
}





/******************************************************
 *	@brief			入力処理
 *	@remarks		1.リスナー作成
 *					2.クリックした時
 *					3.ドラッグ時
 *					4.リスナーの追加
 ******************************************************/
void DebugScene::Input()
{
	
	//1.
	TouchListener* listener = TouchListener::Create();

	//2.
	listener->onTouchBegan = [this](TouchListener::Touch touch)
	{
		m_selectedItem = nullptr;
		SelectObjectFromScreen();
		return true;
	};


	//3.
	listener->onTouchMove = [this](TouchListener::Touch touch)
	{

		CreateInSelectedPoint();
		return true;
	};

	//4.
	GLOBAL::Global::m_eventDispatcher->AddEventListener(listener);
}





/***********************************************************************
 *	@brief			Object選択処理
 *	@remarks		FbxにRay情報を渡すことで,そのRayが当たったかどうか判定する
 ************************************************************************/
bool DebugScene::SelectObjectFromScreen()
{

	D3DXVECTOR2 mousePos = GLOBAL::Global::m_input->GetMousePosition();

	TCHAR str[20];

	RayCastData ray;
	ray.start = GetCamera()->m_transform->m_position;
	ray.dir = GetCamera()->ScreenPointToRay(mousePos.x, mousePos.y);


	Fbx* selectedItem = nullptr;


	//Sceneの子供でFbxのObjectにRay情報を渡す。
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{

		selectedItem = dynamic_cast<Fbx*>(m_transform->m_children[i]->m_attachNode);

		if (selectedItem)
		{
			selectedItem->RayCast(&ray);

			//Rayが当たったら
			if (ray.hit)
			{
				m_selectedItem = selectedItem;
				OutputDebugString(selectedItem->m_nodeName);
				return true;
			}

		}

	}

	return false;
}





/******************************************************************************
 *	@brief		オブジェクトの作成と移動
 *	@remark		m_selectedItemあり：m_selectedItemの移動
 *				m_selectedItemなし：オブジェクトを生成し,m_selectedItemに入れる
 ******************************************************************************/
void DebugScene::CreateInSelectedPoint()
{

	//---エディットボタン上では作成しないように。
	if (m_editButton->CheckCursorOnToggleButton())
	{

		return;
	}

	D3DXVECTOR3* result = new D3DXVECTOR3();
	Coordinate coordinate;
	D3DXVECTOR2 mousePos = GLOBAL::Global::m_input->GetMousePosition();


	//---m_selectedItemあり
	if (m_selectedItem)
	{
		result = coordinate.ScreenToXZPlane(result, mousePos.x, mousePos.y, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT, &GetCamera()->GetView(), &GetCamera()->GetProjection());

		m_selectedItem->m_transform->m_position = *result;

	}
	//---m_selectedItemなし
	else
	{
		result = coordinate.ScreenToXZPlane(result, mousePos.x, mousePos.y, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT, &GetCamera()->GetView(), &GetCamera()->GetProjection());

		Fbx* fbx = Fbx::Create("Assets/FBX/TestCube.fbx");
		fbx->m_transform->m_position = *result;
		fbx->m_transform->SetParent(this->m_transform);
		PhysicsBody* body = PhysicsBody::CreateBox(m_physicsWorld, fbx, 1.0f);
		m_selectedItem = fbx;
	}


	R_SAFE_DELETE(result);
}





/******************************************************
 *	@brief			選択したItemの上下移動
 ******************************************************/
void DebugScene::ItemUpDown()
{

	if (GLOBAL::Global::m_input->IsKeyPressed(DIK_W)
		&& m_selectedItem != nullptr)
	{
		m_selectedItem->m_transform->Translate(Transform::UP, m_selectedItem->m_transform->m_scale.y);
	}

	if (GLOBAL::Global::m_input->IsKeyPressed(DIK_S)
		&& m_selectedItem != nullptr)
	{
		m_selectedItem->m_transform->Translate(Transform::DOWN, m_selectedItem->m_transform->m_scale.y);
	}

}


/*********************************************************************
 *	@brief			debugモードを有効にするか
 *	@remarks		SceneのInitで呼ぶ。
 *					TRUE	：	debugモード有効にする
 *					FALSE	：	debugモード無効にする
 *********************************************************************/
void DebugScene::SetActive(bool active)
{

	if (active)
	{
		for (int i = 0; i < m_debugItemList.size(); i++)
		{
			if (m_debugItemList[i]->m_nodeName == "ToggleButton" || m_debugItemList[i]->m_nodeName == "Label")
			{
				m_debugItemList[i]->SetOpacity(255);
				m_debugItemList[i]->m_isActive = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < m_debugItemList.size(); i++)
		{
			if (m_debugItemList[i]->m_nodeName == "ToggleButton" || m_debugItemList[i]->m_nodeName == "Label")
			{
				m_debugItemList[i]->SetOpacity(0);
				m_debugItemList[i]->m_isActive = false;
			}
		}
	}

	m_isActive = active;

}


/***********************************************************
 *	@brief		編集モードを切り替えるボタンの作成
 ***********************************************************/
void DebugScene::CreateEditButton()
{
	m_editButton = ToggleButton::Create("Assets/UI/PlayMode.png", "Assets/UI/Debug.png");
	m_editButton->m_transform->m_anchorPoint = D3DXVECTOR3(0, 0, 0);
	m_editButton->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_fps->m_transform->m_position.y + m_editButton->m_transform->m_size.y * m_editButton->m_transform->m_scale.y, 0);
	m_editButton->m_transform->SetParent(this->m_transform);
	m_editButton->SetNodeName("EditButton");

	//---Buttonを押したとき
	m_editButton->onTouchBegan = [this](void)
	{

		m_isEdit = !m_isEdit;
		m_physicsWorld->SetActive(!m_physicsWorld->GetActive());

		//---DebugMode時
		if (m_isEdit)
		{
			for (int i = 0; i < m_debugItemList.size(); i++)
			{
				if (m_debugItemList[i]->m_nodeName != "EditButton")
				{
					m_debugItemList[i]->m_isActive = true;
				}
			}
		}
		//---DebugModeでない時
		else
		{
			for (int i = 0; i < m_debugItemList.size(); i++)
			{
				if (m_debugItemList[i]->m_nodeName != "EditButton")
				{
					m_debugItemList[i]->m_isActive = false;
				}
			}

		}


	};

	//DebugItemListに追加。
	m_debugItemList.push_back(m_editButton);
}





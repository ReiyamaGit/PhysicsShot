/***************************************************************
 *	@file				DebugScene.h
 *	@brief				DebugScene Class ��`
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
const float TRANSLATE_SPEED = 0.3f;				//�ړ����x
const float ROTATE_SPEED = 0.8f;				//��]���x
const int FONT_SIZE = 20;
const float BUTTON_TEXTURE_SIZE_H = 64;
const float CAMERA_ZOOM_STD_POINT = 20;
const float CAMERA_SENSITYVITY = 0.01f;


/********************************************************
 *	@brief		�R���X�g���N�^ - Constructor
 ********************************************************/
DebugScene::DebugScene()
	: m_selectedItem(nullptr)
	, m_fps(nullptr)
	, m_isEdit(false)
{
	m_debugItemList.clear();
}



/********************************************************
 *	@brief		�f�X�g���N�^ - Desturctor
 ********************************************************/
DebugScene::~DebugScene()
{
}




/******************************************************
 *	@brief		���������� - Initialize Process
 ******************************************************/
bool DebugScene::Init()
{
	if (!Scene::Init())
	{
		return false;
	}

	//FPS�\��
	TCHAR str[20];
	sprintf_s(str, TEXT("FPS = %.1f"), GLOBAL::Global::m_scheduler->GetFPS());
	m_fps = Label::Create(str, "", FONT_SIZE);
	m_fps->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, -GLOBAL::Global::WINDOW_HEIGHT / 2.0f, 0);
	m_fps->m_transform->m_anchorPoint = D3DXVECTOR3(0, 0, 0);
	m_fps->m_transform->SetParent(this->m_transform);
	m_fps->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_fps);

	//Mode�؂�ւ�Button
	CreateEditButton();

	//x���W
	sprintf_s(str, TEXT("x = %.1f"), 0);
	m_xPos = Label::Create(str, "", FONT_SIZE);
	m_xPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_editButton->m_transform->m_position.y + BUTTON_TEXTURE_SIZE_H, 0);
	m_xPos->m_transform->SetParent(this->m_transform);
	m_xPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_xPos);

	//y���W
	sprintf_s(str, TEXT("y = %.1f"), 0);
	m_yPos = Label::Create(str, "", FONT_SIZE);
	m_yPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_xPos->m_transform->m_position.y + FONT_SIZE, 0);
	m_yPos->m_transform->SetParent(this->m_transform);
	m_yPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_yPos);


	//z���W
	sprintf_s(str, TEXT("z = %.1f"), 0);
	m_zPos = Label::Create(str, "", FONT_SIZE);
	m_zPos->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_yPos->m_transform->m_position.y + FONT_SIZE, 0);
	m_zPos->m_transform->SetParent(this->m_transform);
	m_zPos->m_transform->m_attachNode->m_isActive = false;
	m_debugItemList.push_back(m_zPos);

	//DebugScene���͏���
	DebugScene::Input();

	return true;
}


/******************************************************
 *	@brief			�X�V����
 *	@param[delta]	�ŏI�t���[�����s����
 ******************************************************/
bool DebugScene::Update(float delta)
{
	//FPS�̕\��
	if (m_fps)
	{
		TCHAR str[20];
		sprintf_s(str, TEXT("FPS = %.1f"), GLOBAL::Global::m_scheduler->GetFPS());
		m_fps->SetString(str);
	}


	//�I�����ꂽItem�̍��W�\��
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


	//DebugMode��
	if (m_isEdit)
	{
		ItemUpDown();

		//Camera�̃R���g���[������
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
 *	@brief			���͏���
 *	@remarks		1.���X�i�[�쐬
 *					2.�N���b�N������
 *					3.�h���b�O��
 *					4.���X�i�[�̒ǉ�
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
 *	@brief			Object�I������
 *	@remarks		Fbx��Ray����n�����Ƃ�,����Ray�������������ǂ������肷��
 ************************************************************************/
bool DebugScene::SelectObjectFromScreen()
{

	D3DXVECTOR2 mousePos = GLOBAL::Global::m_input->GetMousePosition();

	TCHAR str[20];

	RayCastData ray;
	ray.start = GetCamera()->m_transform->m_position;
	ray.dir = GetCamera()->ScreenPointToRay(mousePos.x, mousePos.y);


	Fbx* selectedItem = nullptr;


	//Scene�̎q����Fbx��Object��Ray����n���B
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{

		selectedItem = dynamic_cast<Fbx*>(m_transform->m_children[i]->m_attachNode);

		if (selectedItem)
		{
			selectedItem->RayCast(&ray);

			//Ray������������
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
 *	@brief		�I�u�W�F�N�g�̍쐬�ƈړ�
 *	@remark		m_selectedItem����Fm_selectedItem�̈ړ�
 *				m_selectedItem�Ȃ��F�I�u�W�F�N�g�𐶐���,m_selectedItem�ɓ����
 ******************************************************************************/
void DebugScene::CreateInSelectedPoint()
{

	//---�G�f�B�b�g�{�^����ł͍쐬���Ȃ��悤�ɁB
	if (m_editButton->CheckCursorOnToggleButton())
	{

		return;
	}

	D3DXVECTOR3* result = new D3DXVECTOR3();
	Coordinate coordinate;
	D3DXVECTOR2 mousePos = GLOBAL::Global::m_input->GetMousePosition();


	//---m_selectedItem����
	if (m_selectedItem)
	{
		result = coordinate.ScreenToXZPlane(result, mousePos.x, mousePos.y, GLOBAL::Global::WINDOW_WIDTH, GLOBAL::Global::WINDOW_HEIGHT, &GetCamera()->GetView(), &GetCamera()->GetProjection());

		m_selectedItem->m_transform->m_position = *result;

	}
	//---m_selectedItem�Ȃ�
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
 *	@brief			�I������Item�̏㉺�ړ�
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
 *	@brief			debug���[�h��L���ɂ��邩
 *	@remarks		Scene��Init�ŌĂԁB
 *					TRUE	�F	debug���[�h�L���ɂ���
 *					FALSE	�F	debug���[�h�����ɂ���
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
 *	@brief		�ҏW���[�h��؂�ւ���{�^���̍쐬
 ***********************************************************/
void DebugScene::CreateEditButton()
{
	m_editButton = ToggleButton::Create("Assets/UI/PlayMode.png", "Assets/UI/Debug.png");
	m_editButton->m_transform->m_anchorPoint = D3DXVECTOR3(0, 0, 0);
	m_editButton->m_transform->m_position = D3DXVECTOR3(-GLOBAL::Global::WINDOW_WIDTH / 2.0f, m_fps->m_transform->m_position.y + m_editButton->m_transform->m_size.y * m_editButton->m_transform->m_scale.y, 0);
	m_editButton->m_transform->SetParent(this->m_transform);
	m_editButton->SetNodeName("EditButton");

	//---Button���������Ƃ�
	m_editButton->onTouchBegan = [this](void)
	{

		m_isEdit = !m_isEdit;
		m_physicsWorld->SetActive(!m_physicsWorld->GetActive());

		//---DebugMode��
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
		//---DebugMode�łȂ���
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

	//DebugItemList�ɒǉ��B
	m_debugItemList.push_back(m_editButton);
}





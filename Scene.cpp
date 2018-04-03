/******************************************************
 *	@file		Scene.cpp
 *	@brief		Scene	����
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/



//preproccesor--------------------------------
#include "Scene.h"
#include "Fbx.h"
#include "Camera.h"
#include "Light.h"
#include "Scheduler.h"
#include "Draw3DNode.h"
#include "ToggleButton.h"
#include "TouchListener.h"

#include <algorithm>




/******************************************************
 *	@brief		�R���X�g���N�^
 ******************************************************/
Scene::Scene()
:main_camera_(nullptr)
,m_physicsWorld(nullptr)
{
	
	m_referenceCount = 0;
}



/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
Scene::~Scene()
{
	R_SAFE_DELETE(main_camera_);
	R_SAFE_DELETE(m_physicsWorld);
	
}





/******************************************************
*	@brief		����������
*	@return		true: ����	false:���s
*	@remarks	�q�m�[�h�̏��������������邽�ߍŏ���
*				Node�N���X��Init()���Ă�ł���
******************************************************/
bool Scene::Init()
{
	if (!Node::Init())
	{
		return false;
	}


	CountRetain();


	Camera* camera = Camera::Create();
	camera->m_transform->SetParent(this->m_transform);
	camera->m_transform->m_position = D3DXVECTOR3(0, 5, -10);
	camera->Update();
	main_camera_ = camera;
	GLOBAL::Global::m_mainCamera = camera;




	Light* light = Light::Create();
	light->m_transform->SetParent(this->m_transform);





	if (!camera)
	{
		R_ASSERT(camera);
	}


	if (!light)
	{
		R_ASSERT(light);
	}



	return true;
}



/****************************************************************************
 *	@brief		�X�V����
 *	@return		PlayScene��Update���ĂԂ�
 true �� �Ă�		false �� �Ă΂Ȃ�

 *	@remarks	�ҏW���[�h�̏ꍇ�͂��ׂĂ�Update���X���[������
 *				�ŏ���Node��Update���Ă�	��	�q�m�[�hUpdate����
 *				FPS��\������
 *				�`�揇�\�[�g
 *				isEdit	true �FPlayScene��Update���Ă΂Ȃ�
 *						false�FPlayScene��Update���Ă�
 ****************************************************************************/
bool Scene::Update(float delta)
{
	if (!Node::Update(delta))
	{
		return false;
	}


	DrawSort(delta);



	return true;
}




/**********************************************
 *	@brief		�`�揇�\�[�g
 *	@param		�O�t���[������̍X�V����
 **********************************************/
void Scene::DrawSort(float delta)
{



	for (INT i = 0; i < m_transform->m_children.size(); i++)
	{
		m_transform->m_children[i]->m_attachNode->Update(delta);

		//---�\�[�g����m�[�h�͋����𑪂�
		if (m_transform->m_children[i]->m_attachNode->GetIsSort())
		{
			//---�J�����ƃm�[�h�̋���
			D3DXVECTOR3 v = GetCamera()->m_transform->m_position - m_transform->m_children[i]->m_position;
			m_transform->m_children[i]->m_attachNode->SetDistance(D3DXVec3Length(&v));
		}
		else
		{
			//---�\�[�g���Ȃ��Ĉ�ԑO�Ɏ����Ă��������
			m_transform->m_children[i]->m_attachNode->SetDistance(999999);
		}
	}

	//---�J��������̋������������Ƀ\�[�g
	std::sort(m_transform->m_children.begin(), m_transform->m_children.end(), NodeSort());
}



// Scene* Scene::CreateWithPhysicsWorld()
//{
//	 Scene* ret = new Scene();
//	 
//	 if (ret && ret->Init())
//	 {
//		 ret->AutoRelease();
//		 delete ret;
//	 }
//
//
//	 R_SAFE_DELETE(ret);
//
//	 return nullptr;
//
//}




 /*Scene* Scene::Create()
 {
	 Scene* ret = new Scene();
	 PhysicsWorld* physicsWorld = new PhysicsWorld();

	 if (ret && ret->Init() && physicsWorld && physicsWorld->Init())
	 {
		 ret->AutoRelease();
		 ret->m_physicsWorld = physicsWorld;
		 return ret;
	 }

	 R_SAFE_DELETE(ret);

	 return nullptr;

 }*/
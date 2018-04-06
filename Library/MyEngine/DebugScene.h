/***************************************************************
 *	@file				DebugScene.h
 *	@brief				DebugScene Class ��`
 *	@date				2018/01/24
 *	@name				hatakeyama
 ***************************************************************/

#ifndef __DEBUGSCENE__
#define __DEBUGSCENE__


//preprocessor--------------------------------------------------
#include "Scene.h"


//prototype----------------------------------------------------
class Node;
class Label;
class ToggleButton;



/***************************************************************
 *	@brief  �p�����[�^�[�Ƃ���\�������pScene Class
 ***************************************************************/
class DebugScene : public Scene
{
//variable---------------------------------------------------------------
private:
	bool m_isEdit;
	float m_camZoomPos;

	
	Label* m_xPos;								//Item��X���W
	Label* m_yPos;								//Item��Y���W
	Label* m_zPos;								//Item��Z���W
	Label* m_xRotation;							//Item��X���p�x
	Label* m_yRotation;							//Item��Y���p�x
	Label* m_zRotation;							//ItemnoZ���p�x
	Label* m_fps;

	ToggleButton* m_editButton;					//�ҏW���[�h�{�^��
	
	std::vector<Node*> m_debugItemList;


	D3DXVECTOR3 m_GameCamPos;
	D3DXVECTOR2 m_SceneCamPos;
	D3DXVECTOR2 m_touchBeginPos;

protected:
	Node*  m_selectedItem;						//�I������Item	

//function-----------------------------------------------------------------
public:

	DebugScene();

	~DebugScene();


	/******************************************************
	 *	@brief			����������
	 ******************************************************/
	virtual bool Init();
	
	
	/******************************************************
	 *	@brief			�X�V����
	 *	@param[delta]	�ŏI�t���[�����s����
	 ******************************************************/
	virtual bool Update(float delta);


	/******************************************************
	 *	@brief			���͏���
	 ******************************************************/
	virtual void Input();



	/******************************************************
	 *	@brief			�I������Item�̉�]
	 ******************************************************/
	void ItemUpDown();
	

	/******************************************************
	 *	@brief			�X�N���[������I�u�W�F�N�g��I��
	 ******************************************************/
	bool SelectObjectFromScreen();


	
	/******************************************************
	 *	@brief			�N���b�N�����Ƃ���ɃI�u�W�F�N�g���쐬
	 ******************************************************/
	void CreateInSelectedPoint();


	
	/******************************************************
	 *	@brief			�ҏW�{�^���̍쐬
	 ******************************************************/
	void CreateEditButton();
	
	
	/******************************************************
	 *	@brief			Debug�V�[����ON�EOFF
	 ******************************************************/
	void SetActive(bool active);


	
};


#endif //!__DEBUGSCENE__
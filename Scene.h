/*******************************************
 *	@file		Scene.h
 *	@brief		Scene class 定義
 *	@data		2017/11/28
 *	@name		hatakeyama
 *******************************************/


#ifndef __SCENE__
#define __SCENE__


//preproccesor----------------------------------
#include "Node.h"
#include "PhysicsWorld.h"


//prototype------------------------------------
class Camera;



class Scene : public Node
{

//struct------------------------------------------
public:
	typedef struct NodeSort
	{
		bool operator() (Transform* a, Transform* b)
		{
			if (a->m_attachNode->GetDistance() > b->m_attachNode->GetDistance())
			{
				return true;
			}

			return false;
		}
	};


//variable----------------------------------------
protected:
	PhysicsWorld* m_physicsWorld;


	
//function----------------------------------------
protected:
	Scene();

public:
	~Scene();


	/*****************************************
	 *	@brief	初期化
	 ****************************************/
	bool Init()override;


	/*****************************************
	*	@brief	更新処理
	****************************************/
	bool Update(float delta) override;


	/*****************************************
	 *	@brief	描画順ソート
	 ****************************************/
	void DrawSort(float delta);


	inline  PhysicsWorld* GetPhysicsWorld() const { return m_physicsWorld; }

//macro-------------------------------------------
public:
	R_SYNTHESIZE(Camera*, main_camera_, Camera);





};


#endif // !__SCENE__
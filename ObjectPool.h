/******************************************************************
 *	@file		ObjectPool.cpp
 *	@brief		�I�u�W�F�N�g�v�[�� template ����
 *	@date		2018/03/21
 *	@name		hatakeyama
 ******************************************************************/

#ifndef __OBJECTPOOL__
#define __OBJECTPOOL__


//preprocessor------------------------------------------------
#include <vector>

template <typename T>
class ObjectPool
{

//variable----------------------------------------------------
private:
	std::vector<T*> m_registerObjects;



//function----------------------------------------------------
public:
	ObjectPool<T>::ObjectPool() { m_registerObjects.clear(); };

	ObjectPool<T>::~ObjectPool() { m_registerObjects.shrink_to_fit(); };

	
	/****************************************************
	 *	@brief		Object�ǉ�
	 ****************************************************/
	 void ObjectPool<T>::AddObject(T* add) { m_registerObjects.push_back(add); };


	 /****************************************************
	  *	@brief		�C���X�^���X��j�󂷂�
	  ****************************************************/
	 void ObjectPool<T>::Destroy() 
	 { 
		 for (int i = m_registerObjects.size() - 1; i >= 0; i--)
				R_SAFE_DELETE(m_registerObjects[i]); 
			
	 };

	

	 unsigned int ObjectPool<T>::GetCount() const { return m_registerObjects.size(); }


};

#endif //!__OBJECTPOOL__
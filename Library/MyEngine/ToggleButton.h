/******************************************************
 *	@file		ToggleButton.h
 *	@brief		ToggleButton ’è‹`
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#ifndef __ToggleButton__
#define __ToggleButton__

#include "Sprite.h"
#include "Constant.h"
#include <functional>



/*****************************************
 *	@brief		ToggleButton Class
 *****************************************/
class ToggleButton:public Sprite
{


//variable--------------------------------
protected:
	LPCSTR m_pushed;
	LPCSTR m_normal;

public:

	typedef std::function<void(void)> callBackTouchBegan;

	callBackTouchBegan onTouchBegan;

	int triggerKey;


//function--------------------------------
public:
	ToggleButton();
	~ToggleButton();

	

	bool Update(float delta) override;

	void Input() ;

	void Load(LPCSTR m_normal, LPCSTR m_pushed);

	
	static ToggleButton* Create(LPCSTR m_normal ,LPCSTR m_pushed);

	bool CheckCursorOnToggleButton();


	inline bool IsNormal(){	return GetSpriteName() == m_normal;	}


};

#endif	//!__ToggleButton__
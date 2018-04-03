#include "TouchListener.h"


TouchListener::TouchListener()
: onTouchBegan(nullptr)
{
	m_referenceCount = 1;
}


TouchListener::~TouchListener()
{
	R_SAFE_DELETE(m_transform);
}


TouchListener* TouchListener::Create()
{
	TouchListener* ret = new TouchListener();

	if (ret)
	{
	
		return ret;
	}

	R_SAFE_DELETE(ret);


	return nullptr;
}


bool TouchListener::Init()
{
	return true;
}



void TouchListener::Input()
{
	Touch touch = Touch();
	touch.position.x = GLOBAL::Global::m_input->GetMousePosition().x;
	touch.position.y = GLOBAL::Global::m_input->GetMousePosition().y;

	if (onTouchBegan != nullptr && GLOBAL::Global::m_input->IsMousePush(0))
	{
		onTouchBegan(touch);
	}


	if (onTouchMove != nullptr && GLOBAL::Global::m_input->IsMouseMove(0))
	{
		onTouchMove(touch);
	}


	if (onTouchEnd != nullptr && GLOBAL::Global::m_input->IsMouseRelease(0))
	{
		
		onTouchEnd(touch);
	}
	
	
}
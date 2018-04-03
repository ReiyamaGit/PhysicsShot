/******************************************************
*	@file		Light.h
*	@brief		Light Class ŽÀ‘•
*	@data		2018/01/10
*	@name		hatakeyama
******************************************************/


#pragma once

#include "Node.h"

class Light:public Node
{
	~Light();
public:
	static Light* Create();
};


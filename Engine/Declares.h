#pragma once
#include "SystemManager.h"
#include "InputManger.h"
#include <cassert>
extern SystemManager* gSystemManager;
extern InputManger* gInputManager;


#define ASSERT(e) 	assert(e);
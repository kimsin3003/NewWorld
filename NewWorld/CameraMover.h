#pragma once
#include "Engine/RGameObject.h"
class CameraMover : public RGameObject
{

public:
	virtual void Start() override;


	virtual void Tick(double deltaTime) override;

};


#pragma once
#include "Engine/RGameObject.h"
class Room : public RGameObject
{
public:

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;

};


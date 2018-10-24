#pragma once
#include "Engine/IGameManager.h"
class GameManager : public IGameManager
{

	virtual void Initialize() override;
	virtual void Tick() override;
};
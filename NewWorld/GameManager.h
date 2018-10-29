#pragma once
#include "Engine/IGameManager.h"
#include <vector>
class GameManager : public IGameManager
{

	virtual void Initialize() override;
	virtual void Tick(double deltaTime) override;

	std::vector<class Room*> m_room;
};
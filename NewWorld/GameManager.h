#pragma once
#include "Engine/IGameManager.h"
#include <vector>
class GameManager : public IGameManager
{

	virtual void Initialize() override;
	virtual void Tick() override;

	std::vector<class Room*> m_room;
};
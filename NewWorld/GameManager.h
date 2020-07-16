#pragma once
#include <vector>
#include "Engine/IGameManager.h"
#include "CameraMover.h"
class GameManager : public IGameManager
{

	virtual void Initialize() override;
	virtual void Tick(double deltaTime) override;

	std::vector<class Room*> m_room;
	CameraMover m_camMover;
};
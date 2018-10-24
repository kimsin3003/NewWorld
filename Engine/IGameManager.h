#pragma once
class IGameManager
{
public:
	virtual void Initialize() = 0;
	virtual void Tick() = 0;
};


#include "LogManager.h"
#include <Windows.h>
#include <iostream>
#include <sstream>

void LogManager::LogError(std::string errorMessage)
{
	std::ostringstream os; 
	os << errorMessage;
	OutputDebugString(os.str().c_str());
}

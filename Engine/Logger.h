#pragma once
#include <string>
#include <fstream>
#include <windows.h>
class Logger
{
public:
	static void Initialize(std::string fileName);
	static void Log(std::string message);
	static void Log(HRESULT hr);
	static void ShowAlert(std::string message);
	static void Finalize();

private:
	static std::string logStack;
	static std::ofstream ofs;
};

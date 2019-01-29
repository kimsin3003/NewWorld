#include "Logger.h"
#include <chrono>
#include <ctime>
#include <comdef.h>


std::string Logger::logStack = "";
std::ofstream Logger::ofs = std::ofstream();

void Logger::Initialize(std::string fileName)
{
	ofs = std::ofstream(fileName, std::ofstream::out);
}

void Logger::Log(std::string message)
{
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	char time[26];
	ctime_s(time, sizeof(time), &end_time);
 	logStack += time;
 	logStack += " - ";
	logStack += message;
	logStack += "\n";
}

void Logger::Log(HRESULT hr)
{
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	char time[26];
	ctime_s(time, sizeof(time), &end_time);
	logStack += time;
	logStack += " - ";
	
	_com_error err(hr);
	std::wstring errMsg = err.ErrorMessage();
	logStack += std::string(errMsg.begin(), errMsg.end());
	logStack += "\n";
}

void Logger::ShowAlert(std::string message)
{
	int msgboxID = MessageBoxA(
		NULL,
		message.c_str(),
		"NewWorld",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	exit(0);
}

Logger::~Logger()
{
	ofs << logStack;
	ofs.close();
}

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
	LPCTSTR errMsg = err.ErrorMessage();
	logStack += errMsg;
	logStack += "\n";
}

void Logger::Finalize()
{
	ofs << logStack;
	ofs.close();
}

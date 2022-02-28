#include "ConsoleLogging.h"
#include <iostream>
#include <Windows.h>

#include <ctime>
#include <time.h>

HANDLE console;
bool isInitialised = false;

clog::LogFlags clog::LogSettings = clog::LogFlags::Time | clog::LogFlags::SourceInfo;

void Init()
{
	if (!isInitialised)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		isInitialised = true;
	}
}

void LayoutPrefix(int line, const std::string& func, const std::string& file, std::string& output)
{
	if ((int)clog::LogSettings != 0)
	{
		if (clog::LogSettings & clog::LogFlags::Time)
		{
			std::time_t t = std::time(nullptr);

			struct tm timeInfo;
			localtime_s(&timeInfo, &t);

			std::string buffer;
			buffer.resize(20);
			int len = strftime(&buffer[0], buffer.size(), "%X", &timeInfo);
			while (len == 0) {
				buffer.resize(buffer.size() * 2);
				len = strftime(&buffer[0], buffer.size(), "%X", &timeInfo);
			}

			output += "[" + buffer +"]";
		}
		if (clog::LogSettings & clog::LogFlags::SourceInfo)
		{
			output += "(Line " + std::to_string(line) + " @ " + func + " @ " + file + ")";
		}
		output += " ";
	}
}

void clog::Error(int line, std::string sourceFunc, std::string sourceFile, std::string message, bool throwException)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED);
	std::string output = "[ERROR] ";
	LayoutPrefix(line, sourceFunc, sourceFile, output);
	std::cout << output << message << "\n";
	if (throwException)
	{
		throw std::exception(message.c_str());
	}
}

void clog::Warning(int line, std::string sourceFunc, std::string sourceFile, std::string message)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
	std::string output = "[WARN] ";
	LayoutPrefix(line, sourceFunc, sourceFile, output);
	std::cout << output << message << "\n";
}

void clog::Info(int line, std::string sourceFunc, std::string sourceFile, std::string message)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::string output = "[INFO] ";
	LayoutPrefix(line, sourceFunc, sourceFile, output);
	std::cout << output << message << "\n";
}

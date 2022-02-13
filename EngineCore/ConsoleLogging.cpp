#include "ConsoleLogging.h"
#include <iostream>
#include <Windows.h>

HANDLE console;
bool isInitialised = false;

void Init()
{
	if (!isInitialised)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		isInitialised = true;
	}
}
void clog::Error(int line, std::string sourceFunc, std::string sourceFile, std::string message, bool throwException)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED);
	std::cout << "[ERROR] (Line " << line << " @ " << sourceFunc << " @ " << sourceFile << ") - " << message << std::endl;
	if (throwException)
	{
		throw std::exception(message.c_str());
	}
}

void clog::Warning(int line, std::string sourceFunc, std::string sourceFile, std::string message)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << "[WARN] (Line " << line << " @ " << sourceFunc << " @ " << sourceFile << ") - " << message << std::endl;
}

void clog::Info(int line, std::string sourceFunc, std::string sourceFile, std::string message)
{
	Init();
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "[INFO] (Line " << line << " @ " << sourceFunc << " @ " << sourceFile << ") - " << message << std::endl;
}

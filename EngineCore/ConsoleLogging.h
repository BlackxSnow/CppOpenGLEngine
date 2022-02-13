#pragma once

#include "Defs.h"

#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#define CLOGINFO __LINE__, __func__, __FILENAME__

#include <string>

/// <summary>
/// Standardised logging format and colouring scheme.
/// </summary>
namespace clog
{
	void Error(int line, std::string sourceFunc, std::string sourceFile, std::string message, bool throwException);
	void Warning(int line, std::string sourceFunc, std::string sourceFile, std::string message);
	void Info(int line, std::string sourceFunc, std::string sourceFile, std::string message);
}
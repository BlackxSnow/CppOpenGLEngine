#pragma once

#include "Defs.h"

#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#define CLOGINFO __LINE__, __func__, __FILENAME__
#define LogInfo(x) clog::Info(CLOGINFO, x);
#define LogWarning(x) clog::Warning(CLOGINFO, x);
#define LogError(x, t) clog::Error(CLOGINFO, x, t);

#include <string>

/// <summary>
/// Standardised logging format and colouring scheme.
/// </summary>
namespace clog
{
	enum class LogFlags
	{
		Time = 1 << 0,
		SourceInfo = 1 << 1
	};

	inline LogFlags operator|(LogFlags a, LogFlags b)
	{
		return static_cast<LogFlags>(static_cast<int>(a) | static_cast<int>(b));
	}
	inline int operator&(LogFlags a, LogFlags b)
	{
		return static_cast<int>(a) & static_cast<int>(b);
	}

	extern LogFlags LogSettings;

	void Error(int line, std::string sourceFunc, std::string sourceFile, std::string message, bool throwException);
	void Warning(int line, std::string sourceFunc, std::string sourceFile, std::string message);
	void Info(int line, std::string sourceFunc, std::string sourceFile, std::string message);
}
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

	/// <summary>
	/// Flags determining which information is prefixed to the message.
	/// </summary>
	extern LogFlags LogSettings;

	/// <summary>
	/// Log an error into the console. Note: It's recommended to use the LogError macro.
	/// </summary>
	/// <param name="line"></param>
	/// <param name="sourceFunc"></param>
	/// <param name="sourceFile"></param>
	/// <param name="message"></param>
	/// <param name="throwException"></param>
	void Error(int line, std::string sourceFunc, std::string sourceFile, std::string message, bool throwException);
	/// <summary>
	/// Log a warning into the console. Note: it's recommended to use the LogWarning macro.
	/// </summary>
	/// <param name="line"></param>
	/// <param name="sourceFunc"></param>
	/// <param name="sourceFile"></param>
	/// <param name="message"></param>
	void Warning(int line, std::string sourceFunc, std::string sourceFile, std::string message);
	/// <summary>
	/// Log info into the console. Note: it's recommended to use the LogInfo macro.
	/// </summary>
	/// <param name="line"></param>
	/// <param name="sourceFunc"></param>
	/// <param name="sourceFile"></param>
	/// <param name="message"></param>
	void Info(int line, std::string sourceFunc, std::string sourceFile, std::string message);
}
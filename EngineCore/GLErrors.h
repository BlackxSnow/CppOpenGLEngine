#pragma once

#include<string>
#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include "ConsoleLogging.h"

namespace gler
{
	/// <summary>
	/// Return the string representation of an openGL error.
	/// </summary>
	/// <param name="error"></param>
	/// <returns></returns>
	std::string GetGLErrorString(GLenum error);
	/// <summary>
	/// Detect and log pending OpenGL errors. Does not log the error messages if silent is true.
	/// </summary>
	/// <param name="lineNumber"></param>
	/// <param name="func"></param>
	/// <param name="file"></param>
	/// <param name="silent"></param>
	void ProcessGLErrors(int lineNumber, std::string func, std::string file, bool silent = false);
}
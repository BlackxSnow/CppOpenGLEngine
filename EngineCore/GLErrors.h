#pragma once

#include<string>
#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include "ConsoleLogging.h"

namespace gler
{
	std::string GetGLErrorString(GLenum error);
	void ProcessGLErrors(int lineNumber, std::string func, std::string file, bool silent = false);
}
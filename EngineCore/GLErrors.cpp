#include "GLErrors.h"


std::string gler::GetGLErrorString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
    default:                   return "Unknown error (" + std::to_string(error) + ")";
    }
}

void gler::ProcessGLErrors(int lineNumber, std::string func, std::string file, bool silent)
{
    int errorCount = 0;
    while (true)
    {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        errorCount++;

        if (!silent)
        {
            clog::Error(lineNumber, func, file, GetGLErrorString(err), false);
        }
    }
    if (errorCount > 0)
    {
        clog::Warning(lineNumber, func, file, std::to_string(errorCount) + " GL errors were processed.");
    }
}

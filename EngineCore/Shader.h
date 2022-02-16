#pragma once

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string GetFileContents(const char* filename);

/// <summary>
/// OpenGL shader object
/// </summary>
class Shader
{
public:
	GLuint ID;
	/// <summary>
	/// Create and initialise the Shader with the shader code in the specified files.
	/// </summary>
	/// <param name="vertFile"></param>
	/// <param name="fragFile"></param>
	Shader(const char* vertFile, const char* fragFile);
	Shader(std::string vertFile, std::string fragFile);
	/// <summary>
	/// Set this shader as the currently active GL program.
	/// </summary>
	void Activate();
	void Delete();

private:
	void CompileErrors(unsigned int shader, const char* type);
	void InitShader(const char* vertFile, const char* fragFile);
};

#pragma once

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string GetFileContents(const char* filename);

/// <summary>
/// OpenGL shader object container.
/// </summary>
class Shader
{
public:
	/// <summary>
	/// Handle for openGL backing object.
	/// </summary>
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
	/// <summary>
	/// Free the underlying OpenGL object.
	/// </summary>
	void Delete();

private:
	/// <summary>
	/// Detect and print compilation errros for this shader.
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="type"></param>
	void CompileErrors(unsigned int shader, const char* type);
	/// <summary>
	/// Initialise the backing openGL data.
	/// </summary>
	/// <param name="vertFile"></param>
	/// <param name="fragFile"></param>
	void InitShader(const char* vertFile, const char* fragFile);
};

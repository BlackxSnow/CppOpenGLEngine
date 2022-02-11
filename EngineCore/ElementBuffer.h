#pragma once

#include<GL/glew.h>

/// <summary>
/// OpenGL index buffer object
/// </summary>
class ElementBuffer
{
public:
	GLuint ID;
	/// <summary>
	/// Create and populate the ElementBuffer with the given indices array with the specified size.
	/// </summary>
	/// <param name="indices"></param>
	/// <param name="size"></param>
	ElementBuffer(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
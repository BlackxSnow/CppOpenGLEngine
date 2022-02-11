#pragma once

#include<GL/glew.h>

/// <summary>
/// OpenGL VertexBuffer object
/// </summary>
class VertexBuffer
{
public:
	GLuint ID;
	/// <summary>
	/// Create and populate the VertexBuffer with the given vertex array of the specified size.
	/// </summary>
	/// <param name="vertices"></param>
	/// <param name="size"></param>
	VertexBuffer(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
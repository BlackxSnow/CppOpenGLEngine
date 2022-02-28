#pragma once

#include<GL/glew.h>
#include "VertexBuffer.h"

/// <summary>
/// OpenGL VertexArrayObject class
/// </summary>
class VertexArray
{
public:
	GLuint ID;
	VertexArray();

	/// <summary>
	/// Link data from a populated VertexBuffer to the specified shader layout.
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="layout"></param>
	/// <param name="componentCount">Number of numeric components for this layout.</param>
	/// <param name="type">GL numeric type of the specified data.</param>
	/// <param name="stride">Size of each vertex within the buffer.</param>
	/// <param name="offset">Byte offset of the layout data from the start of the vertex data.</param>
	void LinkAttrib(VertexBuffer& buffer, GLuint layout, GLuint componentCount, GLenum type, GLsizeiptr stride, void* offset);
	/// <summary>
	/// Bind this as the active OpenGL VAO.
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbind this as the active VAO.
	/// </summary>
	void Unbind();
	/// <summary>
	/// Free the underlying OpenGL object.
	/// </summary>
	void Delete();
};
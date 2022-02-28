#pragma once

#include<GL/glew.h>
#include<vector>

/// <summary>
/// OpenGL index buffer object
/// </summary>
class ElementBuffer
{
public:
	/// <summary>
	/// OpenGL handle for this buffer.
	/// </summary>
	GLuint ID;
	/// <summary>
	/// Create and populate the ElementBuffer with the given indices array with the specified size.
	/// </summary>
	/// <param name="indices"></param>
	/// <param name="size"></param>
	ElementBuffer(std::vector<GLuint> indices);

	/// <summary>
	/// Binds this as the active buffer.
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbinds this as the active buffer.
	/// </summary>
	void Unbind();
	/// <summary>
	/// Destroys the openGL object backing this buffer.
	/// </summary>
	void Delete();
};
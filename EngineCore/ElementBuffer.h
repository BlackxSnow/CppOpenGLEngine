#pragma once

#include<GL/glew.h>
#include<vector>

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
	ElementBuffer(std::vector<GLuint> indices);

	void Bind();
	void Unbind();
	void Delete();
};
#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<vector>

/// <summary>
/// Single vertex of a mesh.
/// </summary>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Colour;
	glm::vec2 UV;

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 colour, glm::vec2 uv);
};

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
	VertexBuffer(std::vector<Vertex> vertices);

	/// <summary>
	/// Bind this as the active OpenGL VertexBuffer.
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbind this as the active VBO.
	/// </summary>
	void Unbind();
	/// <summary>
	/// Free the underlying OpenGL object.
	/// </summary>
	void Delete();
};
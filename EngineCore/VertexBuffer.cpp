#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VertexBuffer::Delete()
{
	glDeleteBuffers(1, &ID);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 colour, glm::vec2 uv)
{
	Position = position;
	Normal = normal;
	Colour = colour;
	UV = uv;
}

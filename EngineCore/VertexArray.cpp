#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

void VertexArray::LinkAttrib(VertexBuffer& buffer, GLuint layout, GLuint componentCount, GLenum type, GLsizeiptr stride, void* offset)
{
	buffer.Bind();
	glVertexAttribPointer(layout, componentCount, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	buffer.Unbind();
}
void VertexArray::Bind()
{
	glBindVertexArray(ID);
}
void VertexArray::Unbind()
{
	glBindVertexArray(0);
}
void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
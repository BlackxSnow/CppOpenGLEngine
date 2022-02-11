#include "Texture.h"

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(Type, ID);
}

void Texture::Unbind()
{
}

void Texture::SendToShader(Shader& shader, const char* uniformName)
{
	GLuint location = glGetUniformLocation(shader.ID, uniformName);
	shader.Activate();
	glUniform1i(location, Slot);
}

Texture::Texture(const char* textureName, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType)
{
	Type = textureType;
	Slot = slot;
	Format = format;
	PixelType = pixelType;

	stbi_set_flip_vertically_on_load(true);
	Data = stbi_load(textureName, &Width, &Height, &ChannelCount, 0);
	
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(textureType, ID);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(textureType, 0, GL_RGBA, Width, Height, 0, format, pixelType, Data);
	glGenerateMipmap(textureType);

	stbi_image_free(Data);
	glBindTexture(textureType, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

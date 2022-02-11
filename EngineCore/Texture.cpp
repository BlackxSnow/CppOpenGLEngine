#include "Texture.h"

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(Type, ID);
}

void Texture::Unbind()
{
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
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(Data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

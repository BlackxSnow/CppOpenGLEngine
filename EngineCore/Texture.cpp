#include "Texture.h"
#include "GLErrors.h"

void Texture::Bind(GLuint offset)
{
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GLTexType, ID);
}

void Texture::Unbind()
{
	glBindTexture(GLTexType, 0);
}

void Texture::SendToShader(Shader& shader, const char* uniformName, GLuint offset)
{
	GLuint location = glGetUniformLocation(shader.ID, uniformName);
	shader.Activate();
	Bind(offset);
	glUniform1i(location, offset);
	gler::ProcessGLErrors(CLOGINFO);
}

Texture::Texture(const char* textureName, const char* textureType, GLenum format, GLenum pixelType, GLenum glTexType)
{
	Type = textureType;
	GLTexType = glTexType;
	Format = format;
	PixelType = pixelType;

	stbi_set_flip_vertically_on_load(true);
	Data = stbi_load(textureName, &Width, &Height, &ChannelCount, 0);
	
	if (Data == nullptr)
	{
		LogError(std::string("Could not load texture: ") + std::string(textureName), true);
	}

	glGenTextures(1, &ID);
	glBindTexture(glTexType, ID);

	glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(glTexType, 0, GL_RGBA, Width, Height, 0, format, pixelType, Data);
	glGenerateMipmap(glTexType);

	stbi_image_free(Data);
	glBindTexture(glTexType, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

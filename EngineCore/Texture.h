#pragma once
#include<stb/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Texture
{
public:
	GLuint ID;
	const char* Type;
	GLenum GLTexType;
	GLuint Slot;

	GLenum Format;
	GLenum PixelType;

	int Width;
	int Height;
	int ChannelCount;
	unsigned char* Data;

	void Bind();
	void Unbind();

	void SendToShader(Shader& shader, const char* uniformName, GLuint unit);

	Texture(const char* textureName, const char* textureType, GLuint slot, GLenum format, GLenum pixelType, GLenum glTexType = GL_TEXTURE_2D);
	~Texture();
};
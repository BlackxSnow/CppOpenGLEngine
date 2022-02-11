#pragma once
#include<stb/stb_image.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include "Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum Type;
	GLuint Slot;

	GLenum Format;
	GLenum PixelType;

	int Width;
	int Height;
	int ChannelCount;
	unsigned char* Data;

	void Bind();
	void Unbind();

	void SendToShader(Shader& shader, const char* uniformName);

	Texture(const char* textureName, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType);
	~Texture();
};
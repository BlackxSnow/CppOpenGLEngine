#pragma once
#include<stb/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

/// <summary>
/// Container for openGL texture object.
/// </summary>
class Texture
{
public:
	/// <summary>
	/// Handler for OpenGL object.
	/// </summary>
	GLuint ID;
	/// <summary>
	/// Intended purpose of this texture (eg. diffuse).
	/// </summary>
	const char* Type;
	/// <summary>
	/// Underlying OpenGL type of this texture.
	/// </summary>
	GLenum GLTexType;

	/// <summary>
	/// GLEnum image colour data format (eg. RGBA).
	/// </summary>
	GLenum Format;
	/// <summary>
	/// GLEnum image data format (eg unsigned int).
	/// </summary>
	GLenum PixelType;

	int Width;
	int Height;
	int ChannelCount;
	unsigned char* Data;

	/// <summary>
	/// Assign this texture as the active texture in the slot specifed by 'offset'.
	/// </summary>
	/// <param name="offset"></param>
	void Bind(GLuint offset);

	/// <summary>
	/// Unbind this texture from the active texture slot.
	/// </summary>
	void Unbind();

	/// <summary>
	/// Assign this texture to the provided shader under 'uniformName'.
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="uniformName"></param>
	/// <param name="offset"></param>
	void SendToShader(Shader& shader, const char* uniformName, GLuint offset);

	Texture(const char* textureName, const char* textureType, GLenum format, GLenum pixelType, GLenum glTexType = GL_TEXTURE_2D);
	~Texture();
};
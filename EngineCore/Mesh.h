#pragma once
#include <vector>
#include <GL/glew.h>
#include <GL/GL.h>
#include <string>
#include <glm/mat4x4.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Event.h"
#include "ShaderData.h"

class Mesh
{

public:
	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<std::shared_ptr<Texture>> Textures;

	unsigned int MaterialIndex;

	VertexArray VAO;

	void UpdateMesh();

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture>>& textures);
	Mesh();

	/// <summary>
	/// Draws this mesh using the provided model matrix, shader, and camera.
	/// </summary>
	/// <param name="modelMatrix"></param>
	/// <param name="shader"></param>
	/// <param name="camera"></param>
	void Draw(glm::mat4& modelMatrix, Shader& shader, Camera& camera, GLuint minTextureSlot, const ShaderColour& sampleType, const glm::vec3& objectColour);
	/// <summary>
	/// Draws this mesh using the provided model matrix, shader, and cam matrix.
	/// </summary>
	/// <param name="modelMatrix"></param>
	/// <param name="shader"></param>
	/// <param name="camMatrix">- combined projection and view matrices.</param>
	void Draw(glm::mat4& modelMatrix, Shader& shader, const glm::mat4& camMatrix, GLuint minTextureSlot, const ShaderColour& sampleType, const glm::vec3& objectColour);
	/// <summary>
	/// Draws this mesh minimally using the provided model matrix and shader. Does not use textures.
	/// </summary>
	/// <param name="modelMatrix"></param>
	/// <param name="shader"></param>
	/// <param name="minimal"></param>
	void DrawMinimal(glm::mat4& modelMatrix, Shader& shader);
};
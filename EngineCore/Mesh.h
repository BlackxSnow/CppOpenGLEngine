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

	void Draw(glm::mat4& modelMatrix, Shader& shader, Camera& camera);

};
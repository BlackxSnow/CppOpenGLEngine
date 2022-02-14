#include "Mesh.h"
#include "SceneObject.h"
#include "GLErrors.h"
#include <glm/gtc/type_ptr.hpp>


void Mesh::UpdateMesh()
{
	VAO.Bind();

	VertexBuffer vertBuffer(Vertices);
	ElementBuffer elementBuffer(Indices);

	VAO.LinkAttrib(vertBuffer, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
	VAO.LinkAttrib(vertBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
	VAO.LinkAttrib(vertBuffer, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));
	VAO.LinkAttrib(vertBuffer, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 9));

	VAO.Unbind();
	vertBuffer.Unbind();
	elementBuffer.Unbind();
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture>>& textures)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	UpdateMesh();
}

Mesh::Mesh() {}

void Mesh::Draw(glm::mat4& modelMatrix, Shader& shader, Camera& camera, GLuint minTextureSlot)
{
	shader.Activate();
	VAO.Bind();

	unsigned int diffuseCount = 0;
	unsigned int specularCount = 0;


	std::string number;
	std::string type;
	gler::ProcessGLErrors(CLOGINFO);
	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		type = Textures[i]->Type;
		if (Textures[i]->Type == "diffuse")
		{
			number = std::to_string(diffuseCount++);
		}
		else if (Textures[i]->Type == "specular")
		{
			number = std::to_string(specularCount++);
		}
		std::string key = type + number;
		Textures[i]->SendToShader(shader, key.c_str(), i + minTextureSlot);
		gler::ProcessGLErrors(CLOGINFO);
	}

	glm::mat4 view;
	glm::mat4 proj;
	camera.BuildMatrices(&view, &proj);
	glm::mat4 renderMatrix = proj * view * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	gler::ProcessGLErrors(CLOGINFO);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "renderMatrix"), 1, GL_FALSE, glm::value_ptr(renderMatrix));
	gler::ProcessGLErrors(CLOGINFO);

	unsigned int useTextures = Textures.size() > 0 ? 1 : 0;

	glUniform1ui(glGetUniformLocation(shader.ID, "useTextures"), useTextures);

	gler::ProcessGLErrors(CLOGINFO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	gler::ProcessGLErrors(CLOGINFO);
}

void Mesh::Draw(glm::mat4& modelMatrix, Shader& shader, const glm::mat4& camMatrix, GLuint minTextureSlot)
{
	shader.Activate();
	VAO.Bind();

	unsigned int diffuseCount = 0;
	unsigned int specularCount = 0;


	std::string number;
	std::string type;
	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		type = Textures[i]->Type;
		if (Textures[i]->Type == "diffuse")
		{
			number = std::to_string(diffuseCount++);
		}
		else if (Textures[i]->Type == "specular")
		{
			number = std::to_string(specularCount++);
		}
		std::string key = type + number;
		Textures[i]->SendToShader(shader, key.c_str(), i + minTextureSlot);
	}

	glm::mat4 renderMatrix = camMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "renderMatrix"), 1, GL_FALSE, glm::value_ptr(renderMatrix));

	unsigned int useTextures = Textures.size() > 0 ? 1 : 0;

	glUniform1ui(glGetUniformLocation(shader.ID, "useTextures"), useTextures);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawMinimal(glm::mat4& modelMatrix, Shader& shader)
{
	shader.Activate();
	VAO.Bind();

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

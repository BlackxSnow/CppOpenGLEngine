#include "Mesh.h"
#include "SceneObject.h"
#include <glm/gtc/type_ptr.hpp>

COMPONENT_DEFINITION(Component, Mesh)

Mesh::Mesh(SceneObject* attachedObject, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture>>& textures) : Component(attachedObject)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	VAO.Bind();

	VertexBuffer vertBuffer(vertices);
	ElementBuffer elementBuffer(indices);

	VAO.LinkAttrib(vertBuffer, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
	VAO.LinkAttrib(vertBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
	VAO.LinkAttrib(vertBuffer, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));
	VAO.LinkAttrib(vertBuffer, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(GLfloat) * 9));

	VAO.Unbind();
	vertBuffer.Unbind();
	elementBuffer.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
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
		Textures[i]->SendToShader(shader, key.c_str(), i);
		Textures[i]->Bind();
	}

	glm::vec3 camPos = camera.GetSceneObject()->GetTransform()->GetWorldPosition();
	glUniform3f(glGetUniformLocation(shader.ID, "camPosition"), camPos.x, camPos.y, camPos.z);

	//model = glm::rotate(model, glm::radians(90.0f * deltaTime), glm::vec3(0, 1, 0));
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model = BuildModelMatrix();
	camera.BuildMatrices(&view, &proj);
	glm::mat4 renderMatrix = proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "renderMatrix"), 1, GL_FALSE, glm::value_ptr(renderMatrix));

	OnRender.Invoke(shader, camera);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

glm::mat4& Mesh::BuildModelMatrix()
{
	glm::mat4 result = glm::translate(glm::mat4(1), GetSceneObject()->GetTransform()->GetWorldPosition()) * glm::toMat4(GetSceneObject()->GetTransform()->GetWorldRotation());
	return result;
}

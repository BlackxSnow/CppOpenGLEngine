#include "Renderer.h"
#include "ConsoleLogging.h"
#include "SceneObject.h"
#include "Conversion.h"
#include "GLErrors.h"
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"
#include "EngineData.h"

COMPONENT_DEFINITION(Component, Renderer)

void Renderer::Draw(Camera& camera)
{
	if (GLShader == nullptr)
	{
		clog::Error(CLOGINFO, "GLShader has not been set for this renderer!", true);
	}
	GLShader->Activate();
	glm::mat4 modelMatrix = GetSceneObject()->GetTransform()->BuildModelMatrix();

	glm::vec3 camPos = camera.GetSceneObject()->GetTransform()->GetWorldPosition();
	glUniform3f(glGetUniformLocation(GLShader->ID, "camPosition"), camPos.x, camPos.y, camPos.z);
	gler::ProcessGLErrors(CLOGINFO);
	if (ShadowCasterMatrices->size() > 0)
	{
		GLuint loc = glGetUniformLocation(GLShader->ID, "lightMatrices");
		glUniformMatrix4fv(loc, ShadowCasterMatrices->size(), GL_FALSE, glm::value_ptr(ShadowCasterMatrices->data()[0]));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, ShadowMapArray);
		glUniform1i(glGetUniformLocation(GLShader->ID, "LightDepthMaps"), 0);
	}
	gler::ProcessGLErrors(CLOGINFO);

	OnRender.Invoke(*GLShader, camera);

	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->Draw(modelMatrix, *GLShader, camera, 1);
		gler::ProcessGLErrors(CLOGINFO);
	}
}

void Renderer::Draw(Shader& shader, const glm::vec3 camPos, const glm::mat4 camMatrix)
{
	if (GLShader == nullptr)
	{
		clog::Error(CLOGINFO, "GLShader has not been set for this renderer!", true);
	}
	glm::mat4 modelMatrix = GetSceneObject()->GetTransform()->BuildModelMatrix();
	glUniform3f(glGetUniformLocation(GLShader->ID, "camPosition"), camPos.x, camPos.y, camPos.z);

	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->Draw(modelMatrix, shader, camMatrix, 0);
	}
}

void Renderer::Draw(const glm::vec3 camPos, const glm::mat4 camMatrix)
{
	if (GLShader == nullptr)
	{
		clog::Error(CLOGINFO, "GLShader has not been set for this renderer!", true);
	}
	glm::mat4 modelMatrix = GetSceneObject()->GetTransform()->BuildModelMatrix();
	glUniform3f(glGetUniformLocation(GLShader->ID, "camPosition"), camPos.x, camPos.y, camPos.z);

	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->Draw(modelMatrix, *GLShader, camMatrix, 0);
	}
}

void Renderer::Draw(Shader& shader, bool minimal)
{
	glm::mat4 modelMatrix = GetSceneObject()->GetTransform()->BuildModelMatrix();

	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		if (minimal)
		{
			mesh->DrawMinimal(modelMatrix, shader);
		}
	}
}

void Renderer::ImportMeshesFromOBJ(std::string filePath)
{
	objl::Loader objLoader;

	bool isLoaded = objLoader.LoadFile(filePath);

	if (!isLoaded)
	{
		clog::Error(CLOGINFO, "Unable to load file at " + filePath, true);
	}

	objl::Mesh* currentMesh;
	Meshes.clear();

	for (int i = 0; i < objLoader.LoadedMeshes.size(); i++)
	{
		currentMesh = &objLoader.LoadedMeshes[i];
		std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
		
		std::vector<Vertex> vertices;

		for (objl::Vertex vert : currentMesh->Vertices)
		{
			vertices.emplace_back(cvrt::Vec3(vert.Position), cvrt::Vec3(vert.Normal), VERT_COLOUR_DEFAULT, cvrt::Vec2(vert.TextureCoordinate));
		}

		mesh->Vertices = vertices;
		mesh->Indices = currentMesh->Indices;
		mesh->UpdateMesh();
		Meshes.push_back(mesh);
	}
}

void Register(Renderer* renderer)
{
	Renderers.push_back(renderer);
}

void Deregister(Renderer* renderer)
{
	Renderers.remove(renderer);
}

Renderer::Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Mesh>> meshes) : Component(attachedObject)
{
	Meshes = meshes;
	GLShader = shader;
	Register(this);
}

Renderer::Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader) : Component(attachedObject)
{
	GLShader = shader;
	Register(this);
}

Renderer::~Renderer()
{
	Deregister(this);
}

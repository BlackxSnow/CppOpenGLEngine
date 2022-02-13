#include "Renderer.h"
#include "ConsoleLogging.h"
#include "SceneObject.h"
#include "Conversion.h"

COMPONENT_DEFINITION(Component, Renderer)

void Renderer::Draw(Camera& camera)
{
	if (GLShader == nullptr)
	{
		clog::Error(CLOGINFO, "GLShader has not been set for this renderer!", true);
	}
	glm::mat4 modelMatrix = GetSceneObject()->GetTransform()->BuildModelMatrix();

	glm::vec3 camPos = camera.GetSceneObject()->GetTransform()->GetWorldPosition();
	glUniform3f(glGetUniformLocation(GLShader->ID, "camPosition"), camPos.x, camPos.y, camPos.z);

	OnRender.Invoke(*GLShader, camera);

	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->Draw(modelMatrix, *GLShader, camera);
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

Renderer::Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Mesh>> meshes) : Component(attachedObject)
{
	Meshes = meshes;
	GLShader = shader;
}

Renderer::Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader) : Component(attachedObject)
{
	GLShader = shader;
}

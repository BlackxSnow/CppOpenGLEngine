#pragma once

#pragma once

#include "Component.h"
#include "Mesh.h"

class Renderer : public Component
{
	COMPONENT_DECLARATION(Renderer)

public:
	std::vector<std::shared_ptr<Mesh>> Meshes;
	// Replace with a vector of Material (class containing a pre-set shader)
	std::shared_ptr<Shader> GLShader;

	Event<Shader&, Camera&> OnRender;

	void Draw(Camera& camera);
	void ImportMeshesFromOBJ(std::string filePath);

	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Mesh>> meshes);
	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader);
};
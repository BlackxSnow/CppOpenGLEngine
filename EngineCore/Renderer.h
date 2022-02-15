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

	GLenum ShadowMapCullingMode = GL_FRONT;

	Event<Shader&, Camera&> OnRender;

	/// <summary>
	/// Draws all meshes with the renderer's shader and the provided camera.
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);
	void Draw(Shader& shader, const glm::vec3 camPos, const glm::mat4 camMatrix);
	/// <summary>
	/// Draws all meshes using the provided position and matrix.
	/// </summary>
	/// <param name="camPos"></param>
	/// <param name="camMatrix">- combined view and projection matrices.</param>
	void Draw(const glm::vec3 camPos, const glm::mat4 camMatrix);
	/// <summary>
	/// Draws all meshes with the provided shader. If minimal is true, does not call event and reduces shader data assignment.
	/// NOTE: Minimal = false has no current functionality
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="minimal"></param>
	void Draw(Shader& shader, bool minimal);
	void ImportMeshesFromOBJ(std::string filePath);

	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Mesh>> meshes);
	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader);
	~Renderer();
};
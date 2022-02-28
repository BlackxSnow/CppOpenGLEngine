#pragma once

#pragma once

#include "Component.h"
#include "Mesh.h"
#include "ShaderData.h"

/// <summary>
/// Rendering component for objects with mesh data.
/// </summary>
class Renderer : public Component
{
	COMPONENT_DECLARATION(Renderer)

public:
	/// <summary>
	/// The list of meshes used by this renderer.
	/// </summary>
	std::vector<std::shared_ptr<Mesh>> Meshes;
	// TODO Replace with a vector of Material (class containing a pre-set shader)
	std::shared_ptr<Shader> GLShader;

	/// <summary>
	/// Culling mode for rendering to shadow maps. GL_Front is preferred when viable (ie. when the mesh has backfaces).
	/// </summary>
	GLenum ShadowMapCullingMode = GL_FRONT;

	/// <summary>
	/// Determines what colour the shader will sample from.
	/// </summary>
	ShaderColour ColourSetting = ShaderColour::Texture;
	glm::vec3 Colour = glm::vec3(0.7,0.7,0.7);

	/// <summary>
	/// Invoked when this object is called for rendering.
	/// </summary>
	Event<Shader&, Camera&> OnRender;

	/// <summary>
	/// Draws all meshes with the renderer's shader and the provided camera.
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

	/// <summary>
	/// Draw all meshes with the provided shader, postion, and combination view and projection matrix.
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="camPos"></param>
	/// <param name="camMatrix"></param>
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

	/// <summary>
	/// Fill meshes with data from an external OBJ file.
	/// </summary>
	/// <param name="filePath"></param>
	void ImportMeshesFromOBJ(std::string filePath);

	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Mesh>> meshes);
	Renderer(SceneObject* attachedObject, std::shared_ptr<Shader> shader);
	~Renderer();
};
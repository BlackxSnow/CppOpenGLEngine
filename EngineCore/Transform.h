#pragma once

#include<glm/vec3.hpp>
#include<glm/gtc/quaternion.hpp>
#include<vector>

#include "Component.h"

class Transform : public Component
{
	COMPONENT_DECLARATION(Transform)
public:
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);
	glm::quat GetRotation();
	void SetRotation(glm::quat quaternion);
	glm::vec3 GetScale();
	void SetScale(glm::vec3 scale);

	/// <summary>
	/// Return a matrix representing the model's total transform (Including parents)
	/// </summary>
	/// <returns></returns>
	glm::mat4& BuildModelMatrix();

	/// <summary>
	/// Return the world position of the transform.
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetWorldPosition();
	/// <summary>
	/// Return the total rotation of the transform.
	/// </summary>
	/// <returns></returns>
	glm::quat GetWorldRotation();
	/// <summary>
	/// Return the normalized vector pointing forward.
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetForwardVector();

	/// <summary>
	/// Translate the transform by the specified amount
	/// </summary>
	/// <param name="translation"></param>
	void Translate(glm::vec3 translation);
	/// <summary>
	/// Rotate the transform by the given Euler values.
	/// </summary>
	/// <param name="euler"></param>
	void Rotate(glm::vec3 euler);

	/// <summary>
	/// Return the hierarchical parent transform if there is one.
	/// </summary>
	/// <returns></returns>
	Transform* GetParent();

	Transform(SceneObject* attachedTo);
	Transform(SceneObject* attachedTo, glm::vec3 pos, glm::quat rot, Transform* parent = nullptr);

private:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	Transform* Parent;
};
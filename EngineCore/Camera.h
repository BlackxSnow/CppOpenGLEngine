#pragma once

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Component.h"
#include "Defs.h"

/// <summary>
/// Graphics rendering component. At least one is required.
/// </summary>
class Camera : public Component
{
	COMPONENT_DECLARATION(Camera)

public:
	// TODO refactor this to private with getters and setters. These should not be modifable on their own.
	/// <summary>
	/// Field of view of the camera in degrees.
	/// </summary>
	float FovDegrees;
	/// <summary>
	/// Field of view of the camera in radians.
	/// </summary>
	float FovRadians;
	/// <summary>
	/// Distance of near clipping plane.
	/// </summary>
	float NearPlane;
	/// <summary>
	/// Distance of far clipping plane.
	/// </summary>
	float FarPlane;

	Camera(SceneObject* attachedTo, float fov, float near, float far);

	/// <summary>
	/// Construct the view and projection matrices within the provided pointers.
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	void BuildMatrices(OUT glm::mat4* view, OUT glm::mat4* projection);
};
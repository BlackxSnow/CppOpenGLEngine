#pragma once

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Component.h"
#include "Defs.h"

class Camera : public Component
{
	COMPONENT_DECLARATION(Camera)

public:
	float FovDegrees;
	float FovRadians;
	float NearPlane;
	float FarPlane;

	Camera(SceneObject* attachedTo, float fov, float near, float far);

	void BuildMatrices(OUT glm::mat4* view, OUT glm::mat4* projection);
};
#include "Camera.h"
#include "SceneObject.h";

COMPONENT_DEFINITION(Component, Camera)

Camera::Camera(SceneObject* attachedTo, float fovDegrees, float near, float far) : Component(std::move(attachedTo))
{
	FovDegrees = fovDegrees;
	FovRadians = glm::radians(fovDegrees);
	NearPlane = near;
	FarPlane = far;
}

void Camera::BuildMatrices(OUT glm::mat4* view, OUT glm::mat4* projection)
{
	*view = glm::translate(glm::mat4(1), GetSceneObject()->GetTransform()->GetWorldPosition()) * glm::toMat4(GetSceneObject()->GetTransform()->GetWorldRotation());
	*view = glm::inverse(*view);
	*projection = glm::perspective(FovRadians, ((float)windowWidth / windowHeight), NearPlane, FarPlane);
}

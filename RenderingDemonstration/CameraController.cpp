#include "CameraController.h"
#include <Engine.h>
#include <ETime.h>

COMPONENT_DEFINITION(Component, CameraController);

void CameraController::Update()
{
	if (glfwGetKey(Window, GLFW_KEY_A))
	{
		_camTransform->Translate(glm::vec3(-1 * DeltaTime, 0, 0));
	}
	if (glfwGetKey(Window, GLFW_KEY_D))
	{
		_camTransform->Translate(glm::vec3(1 * DeltaTime, 0, 0));
	}
	if (glfwGetKey(Window, GLFW_KEY_W))
	{
		_camTransform->Translate(glm::vec3(0, 0, -1 * DeltaTime));
	}
	if (glfwGetKey(Window, GLFW_KEY_S))
	{
		_camTransform->Translate(glm::vec3(0, 0, 1 * DeltaTime));
	}
}

CameraController::CameraController(SceneObject* attachedTo, Transform* transform) : Component(attachedTo), IUpdatable()
{
	_camTransform = transform;
}

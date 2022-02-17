#include "CameraController.h"
#include <Engine.h>
#include <ETime.h>
#include "InputManager.h"

COMPONENT_DEFINITION(Component, CameraController);

void CameraController::DoMovement()
{
	float currentSpeed = Speed;

	if (glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT))
	{
		currentSpeed *= BoostModifier;
	}

	glm::vec3 forward = GetSceneObject()->GetTransform()->Forward();
	glm::vec3 right = GetSceneObject()->GetTransform()->Right();

	if (glfwGetKey(Window, GLFW_KEY_A))
	{
		_camTransform->Translate(-right * currentSpeed * DeltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_D))
	{
		_camTransform->Translate(right * currentSpeed * DeltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_W))
	{
		_camTransform->Translate(forward * currentSpeed * DeltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_S))
	{
		_camTransform->Translate(-forward * currentSpeed * DeltaTime);
	}
}

void CameraController::DoRotation()
{
	if (_isRotating)
	{
		glm::vec2 delta = GetPointerDelta();

		glm::quat horizontalRotation = glm::angleAxis(-delta.x * RotationSensitivity, glm::vec3(0, 1, 0));

		GetSceneObject()->GetTransform()->SetRotation(horizontalRotation * GetSceneObject()->GetTransform()->GetRotation());

		glm::vec3 vAxis = -GetSceneObject()->GetTransform()->Right();
		glm::quat verticalRotation = glm::angleAxis(delta.y * RotationSensitivity, vAxis);

		GetSceneObject()->GetTransform()->SetRotation(verticalRotation * GetSceneObject()->GetTransform()->GetRotation());
	}
}

void CameraController::Update()
{
	DoRotation();
	DoMovement();
}



CameraController::CameraController(SceneObject* attachedTo, Transform* transform) : Component(attachedTo), IUpdatable()
{
	_camTransform = transform;
	// TODO implement some sort of hash for the name parameter.
	RegisterInputCallback(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, "StartMouseRotation", [this](int mods) {StartMouseRotation(mods); });
	RegisterInputCallback(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, "StopMouseRotation", [this](int mods) {StopMouseRotation(mods); });
}

CameraController::~CameraController()
{
	DeregisterInputCallback(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, "StartMouseRotation");
	DeregisterInputCallback(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, "StopMouseRotation");
}

void CameraController::StartMouseRotation(int mods)
{
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_isRotating = true;
}

void CameraController::StopMouseRotation(int mods)
{
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_isRotating = false;
}

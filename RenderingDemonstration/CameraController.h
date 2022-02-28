#pragma once

#include "Component.h"
#include "Callbacks.h"
#include "Transform.h"
#include "SceneObject.h"

/// <summary>
/// Allows movement of the attached object via WASD and rotation view holding RMB.
/// </summary>
class CameraController : public Component, IUpdatable
{
	COMPONENT_DECLARATION(CameraController)

public:
	/// <summary>
	/// Linear movement speed.
	/// </summary>
	float Speed = 1;
	/// <summary>
	/// Rotation sensitivity due to mouse delta.
	/// </summary>
	float RotationSensitivity = 0.01f;
	/// <summary>
	/// Linear movement modifier when shift is held.
	/// </summary>
	float BoostModifier = 2;

	virtual void Update() override;

	CameraController(SceneObject* attachedTo, Transform* transform);
	~CameraController();

private:
	void DoMovement();
	void DoRotation();

	bool _isRotating = false;
	void StartMouseRotation(int mods);
	void StopMouseRotation(int mods);
	Transform* _camTransform;
};
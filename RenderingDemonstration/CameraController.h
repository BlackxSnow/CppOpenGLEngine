#pragma once

#include "Component.h"
#include "Callbacks.h"
#include "Transform.h"
#include "SceneObject.h"

class CameraController : public Component, IUpdatable
{
	COMPONENT_DECLARATION(CameraController)

public:
	float Speed = 1;
	float RotationSensitivity = 0.01f;
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
#pragma once

#include "Component.h"
#include "Callbacks.h"
#include "Transform.h"
#include "SceneObject.h"

class CameraController : public Component, IUpdatable
{
	COMPONENT_DECLARATION(CameraController)

public:
	virtual void Update() override;

	CameraController(SceneObject* attachedTo, Transform* transform);

private:
	Transform* _camTransform;
};
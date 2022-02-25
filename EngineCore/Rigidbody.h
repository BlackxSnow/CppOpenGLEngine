#pragma once

#include "Component.h"
#include <glm/glm.hpp>

enum PhysicsBehaviour
{
	Dynamic,
	Static
};

class Rigidbody : public Component
{
	COMPONENT_DECLARATION(Rigidbody);

public:
	float Mass;
	glm::vec3 Velocity;
	PhysicsBehaviour PhysicsType;

	Rigidbody(SceneObject* attachedTo);
	Rigidbody(SceneObject* attachedTo, float Mass);
	Rigidbody(SceneObject* attachedTo, float Mass, PhysicsBehaviour type);

	bool UseGravity = true;
};
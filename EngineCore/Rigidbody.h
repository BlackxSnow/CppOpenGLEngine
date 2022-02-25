#pragma once

#include "Component.h"
#include <glm/glm.hpp>

enum class PhysicsBehaviour
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

	void Integrate();

	Rigidbody(SceneObject* attachedTo);
	Rigidbody(SceneObject* attachedTo, float mass);
	Rigidbody(SceneObject* attachedTo, float mass, PhysicsBehaviour type);

	~Rigidbody();

	bool UseGravity = true;
};
#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
	COMPONENT_DECLARATION(SphereCollider);

public:
	glm::vec3 Center;
	float Radius;

	virtual Collision::ColliderType ColliderType() override { return _colliderType; }

	SphereCollider(SceneObject* attachedTo);
	SphereCollider(SceneObject* attachedTo, float radius);
	SphereCollider(SceneObject* attachedTo, glm::vec3 center, float radius);

private:
	Collision::ColliderType _colliderType = Collision::ColliderType::Sphere;
};
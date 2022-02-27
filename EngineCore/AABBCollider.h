#pragma once

#include "Collider.h"

class AABBCollider : public Collider
{
	COMPONENT_DECLARATION(AABBCollider);

public:
	glm::vec3 Center;
	glm::vec3 Extents;

	virtual Collision::ColliderType ColliderType() override { return _colliderType; }

	virtual void RecalculateBounds() override;

	AABBCollider(SceneObject* attachedTo);
	AABBCollider(SceneObject* attachedTo, float apothem);
	AABBCollider(SceneObject* attachedTo, glm::vec3 extents);

private:
	Collision::ColliderType _colliderType = Collision::ColliderType::AABB;
};
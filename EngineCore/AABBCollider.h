#pragma once

#include "Collider.h"

/// <summary>
/// Axis Aligned Bounding Box collider.
/// </summary>
class AABBCollider : public Collider
{
	COMPONENT_DECLARATION(AABBCollider);

public:
	/// <summary>
	/// Center offset from the SceneObject's origin.
	/// </summary>
	glm::vec3 Center;
	/// <summary>
	/// Apothem for each axis of the AABB.
	/// </summary>
	glm::vec3 Extents;

	virtual Collision::ColliderType ColliderType() override { return _colliderType; }

	virtual void RecalculateBounds() override;

	AABBCollider(SceneObject* attachedTo);
	AABBCollider(SceneObject* attachedTo, float apothem);
	AABBCollider(SceneObject* attachedTo, glm::vec3 extents);
	AABBCollider(SceneObject* attachedTo, glm::vec3 center, glm::vec3 extents);

private:
	Collision::ColliderType _colliderType = Collision::ColliderType::AABB;
};
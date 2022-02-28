#pragma once

#include "Collider.h"

/// <summary>
/// Spherical collider class defined by a center and radius.
/// </summary>
class SphereCollider : public Collider
{
	COMPONENT_DECLARATION(SphereCollider);

public:
	/// <summary>
	/// Offset of the collider from the SceneObject origin.
	/// </summary>
	glm::vec3 Center;
	float Radius;

	virtual Collision::ColliderType ColliderType() override { return _colliderType; }
	
	virtual void RecalculateBounds() override;

	SphereCollider(SceneObject* attachedTo);
	SphereCollider(SceneObject* attachedTo, float radius);
	SphereCollider(SceneObject* attachedTo, glm::vec3 center, float radius);

private:
	Collision::ColliderType _colliderType = Collision::ColliderType::Sphere;
};
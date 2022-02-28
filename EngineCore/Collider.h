#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "Physics.h"
#include "Event.h"
#include "Rigidbody.h"

/// <summary>
/// Pure virtual base collider class.
/// </summary>
class Collider : public Component
{
	COMPONENT_DECLARATION(Collider);

public:
	/// <summary>
	/// Event called on collider-collider collision.
	/// Is not physically resolved, unlike Rigidbody.OnCollide.
	/// </summary>
	Event<Collision::CollisionData> OnCollide;

	Rigidbody* GetRigidbody();

	/// <summary>
	/// Recalculate the bounds of the collider.
	/// Should be called after any modification of the collider.
	/// </summary>
	virtual void RecalculateBounds() = 0;

	const Collision::Bounds& GetBounds();

	/// <summary>
	/// Perform post-creation initialisation steps. Necessary due to constructor evaluation order.
	/// </summary>
	void Initialise();

	virtual Collision::ColliderType ColliderType() = 0;
	Collider(SceneObject* attachedTo);
	~Collider();

protected: 
	Rigidbody* _rigidbody;
	/// <summary>
	/// Object space bounding box.
	/// </summary>
	Collision::Bounds _bounds;
};
#pragma once

#include "Component.h"
#include "Physics.h"
#include "Event.h"

#include <vector>
#include <glm/glm.hpp>

class Collider;

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

	Event<Collision::CollisionData> OnCollide;

	/// <summary>
	/// Process the movement of this rigidbody using DeltaTime.
	/// Called within the engine. Manual calling of this function is inadvisable.
	/// </summary>
	void Integrate();

	/// <summary>
	/// Recalculate the bounds of this rigidbody using its colliders.
	/// </summary>
	void RecalculateBounds();

	/// <summary>
	/// Calculate and return the bounds of this rigidbody in world space adjusted for size changes due to rotation.
	/// </summary>
	Collision::Bounds GetWorldBounds();

	/// <summary>
	/// Register a collider to this rigidbody.
	/// </summary>
	/// <param name="col"></param>
	void RegisterCollider(Collider* col);
	/// <summary>
	/// Deregister an existing collider from this rigidbody.
	/// </summary>
	/// <param name="col"></param>
	void DeregisterCollider(Collider* col);

	/// <summary>
	/// Return a reference to the list of colliders.
	/// </summary>
	/// <returns></returns>
	const std::vector<Collider*>& GetColliders();

	Rigidbody(SceneObject* attachedTo);
	Rigidbody(SceneObject* attachedTo, float mass);
	Rigidbody(SceneObject* attachedTo, float mass, PhysicsBehaviour type);

	~Rigidbody();

	bool UseGravity = true;

private:
	Collision::Bounds _objectBounds;
	std::vector<Collider*> _colliders;
};
#include "Rigidbody.h"
#include "SceneObject.h"
#include "Transform.h"
#include "ETime.h"
#include "Physics.h"
#include "EngineData.h"

#include <algorithm>
#include <array>

COMPONENT_DEFINITION(Component, Rigidbody);

void Rigidbody::Integrate(const float delta)
{
	if (PhysicsType != PhysicsBehaviour::Static)
	{
		Velocity += glm::vec3(0, -1, 0) * Physics::Gravity * delta;

		Transform* t = GetSceneObject()->GetTransform();
		t->SetPosition(t->GetPosition() + Velocity * delta);
	}
}

void Rigidbody::RecalculateBounds()
{
	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	for (auto& col : _colliders)
	{
		Collision::Bounds bounds = col->GetBounds();
		min = glm::min(min, bounds.Min);
		max = glm::max(max, bounds.Max);
	}
	_objectBounds = Collision::Bounds(min, max);
}

Collision::Bounds Rigidbody::GetWorldBounds()
{
	glm::vec3& extents = _objectBounds.Extents;
	glm::mat4 model = GetSceneObject()->GetTransform()->BuildModelMatrix();
	std::array<glm::vec3, 8> AABBVerts =
	{
		model * glm::vec4(extents.x, extents.y, extents.z, 1),
		model * glm::vec4(extents.x, extents.y, -extents.z, 1),
		model * glm::vec4(extents.x, -extents.y, extents.z, 1),
		model * glm::vec4(extents.x, -extents.y, -extents.z, 1),
		model * glm::vec4(-extents.x, extents.y, extents.z, 1),
		model * glm::vec4(-extents.x, extents.y, -extents.z, 1),
		model * glm::vec4(-extents.x, -extents.y, extents.z, 1),
		model * glm::vec4(-extents.x, -extents.y, -extents.z, 1),
	};

	glm::vec3 min = AABBVerts[0];
	glm::vec3 max = AABBVerts[0];

	for (int i = 0; i < AABBVerts.size(); i++)
	{
		glm::vec3& vert = AABBVerts[i];
		min = glm::min(vert, min);
		max = glm::max(vert, max);
	}

	return Collision::Bounds(min, max);
}

void Rigidbody::RegisterCollider(Collider* col)
{
	_colliders.push_back(col);
	// TODO replace with SetBoundsDirty and recalculate once on physics step.
	RecalculateBounds();
}

void Rigidbody::DeregisterCollider(Collider* col)
{
	_colliders.erase(std::find(_colliders.begin(), _colliders.end(), col));
	RecalculateBounds();
}

const std::vector<Collider*>& Rigidbody::GetColliders()
{
	return _colliders;
}

Rigidbody::Rigidbody(SceneObject* attachedTo) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = PhysicsBehaviour::Dynamic;
	Mass = 1;
	Rigidbodies.push_back(this);
	RecalculateBounds();
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float mass) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = PhysicsBehaviour::Dynamic;
	Mass = mass;
	Rigidbodies.push_back(this);
	RecalculateBounds();
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float mass, PhysicsBehaviour type) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = type;
	Mass = mass;
	Rigidbodies.push_back(this);
	RecalculateBounds();
}

Rigidbody::~Rigidbody()
{
	Rigidbodies.remove(this);
}
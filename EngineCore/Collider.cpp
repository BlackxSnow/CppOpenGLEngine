#include "Collider.h"
#include "EngineData.h"

COMPONENT_DEFINITION(Component, Collider);

Rigidbody* Collider::GetRigidbody()
{
	return _rigidbody;
}

const Collision::Bounds& Collider::GetBounds()
{
	return _bounds;
}

void Collider::Initialise()
{
	std::shared_ptr<Rigidbody> rigidbody = GetSceneObject()->GetComponent<Rigidbody>();
	RecalculateBounds();
	if (rigidbody != nullptr)
	{
		rigidbody->RegisterCollider(this);
		this->_rigidbody = rigidbody.get();
	}
	else
	{
		_rigidbody = nullptr;
		Colliders.push_back(this);
	}
}

Collider::Collider(SceneObject* attachedTo) : Component(attachedTo)
{
}

Collider::~Collider()
{
	if (_rigidbody == nullptr)
	{
		Colliders.remove(this);
	}
	else
	{
		_rigidbody->DeregisterCollider(this);
	}
}
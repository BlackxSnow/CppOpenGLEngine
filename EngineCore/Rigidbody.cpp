#include "Rigidbody.h"
#include "SceneObject.h"
#include "Transform.h"
#include "ETime.h"
#include "Physics.h"
#include "EngineData.h"

COMPONENT_DEFINITION(Component, Rigidbody);

void Rigidbody::Integrate()
{
	if (PhysicsType != PhysicsBehaviour::Static)
	{
		Velocity += glm::vec3(0, -1, 0) * Physics::Gravity * DeltaTime;

		Transform* t = GetSceneObject()->GetTransform();
		t->SetPosition(t->GetPosition() + Velocity * DeltaTime);
	}
}

Rigidbody::Rigidbody(SceneObject* attachedTo) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = PhysicsBehaviour::Dynamic;
	Mass = 1;
	Rigidbodies.push_back(this);
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float mass) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = PhysicsBehaviour::Dynamic;
	Mass = mass;
	Rigidbodies.push_back(this);
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float mass, PhysicsBehaviour type) : Component(attachedTo)
{
	Velocity = glm::vec3(0);
	PhysicsType = type;
	Mass = mass;
	Rigidbodies.push_back(this);
}

Rigidbody::~Rigidbody()
{
	Rigidbodies.remove(this);
}
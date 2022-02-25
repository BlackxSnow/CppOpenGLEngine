#include "SphereCollider.h"

COMPONENT_DEFINITION(Collider, SphereCollider);

SphereCollider::SphereCollider(SceneObject* attachedTo) : Collider(attachedTo)
{
}

SphereCollider::SphereCollider(SceneObject* attachedTo, float radius) : Collider(attachedTo)
{
	Radius = radius;
	Center = glm::vec3(0, 0, 0);
}

SphereCollider::SphereCollider(SceneObject* attachedTo, glm::vec3 center, float radius) : Collider(attachedTo)
{
	Radius = radius;
	Center = center;
}

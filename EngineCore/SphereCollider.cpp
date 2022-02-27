#include "SphereCollider.h"

COMPONENT_DEFINITION(Collider, SphereCollider);

void SphereCollider::RecalculateBounds()
{
	float extent = sqrt(pow(Radius, 2) * 2);
	_bounds = Collision::Bounds(
		Center + glm::vec3(-extent, -extent, -extent),
		Center + glm::vec3(extent, extent, extent));
}

SphereCollider::SphereCollider(SceneObject* attachedTo) : Collider(attachedTo)
{
	Center = glm::vec3(0,0,0);
	Radius = 1;
	Initialise();
}

SphereCollider::SphereCollider(SceneObject* attachedTo, float radius) : Collider(attachedTo)
{
	Radius = radius;
	Center = glm::vec3(0, 0, 0);
	Initialise();
}

SphereCollider::SphereCollider(SceneObject* attachedTo, glm::vec3 center, float radius) : Collider(attachedTo)
{
	Radius = radius;
	Center = center;
	Initialise();
}

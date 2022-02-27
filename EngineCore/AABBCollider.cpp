#include "AABBCollider.h"

COMPONENT_DEFINITION(Collider, AABBCollider);

void AABBCollider::RecalculateBounds()
{
	_bounds = Collision::Bounds(Center - Extents, Center + Extents);
}

AABBCollider::AABBCollider(SceneObject* attachedTo) : Collider(attachedTo)
{
	Extents = glm::vec3(1,1,1);
	Center = glm::vec3(0,0,0);
	Initialise();
}

AABBCollider::AABBCollider(SceneObject* attachedTo, float apothem) : Collider(attachedTo)
{
	Extents = glm::vec3(apothem, apothem, apothem);
	Center = glm::vec3(0,0,0);
	Initialise();
}

AABBCollider::AABBCollider(SceneObject* attachedTo, glm::vec3 extents) : Collider(attachedTo)
{
	Extents = extents;
	Center = glm::vec3(0,0,0);
	Initialise();
}

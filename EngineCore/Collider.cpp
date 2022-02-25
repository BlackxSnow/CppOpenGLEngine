#include "Collider.h"
#include "EngineData.h"

COMPONENT_DEFINITION(Component, Collider);

Collider::Collider(SceneObject* attachedTo) : Component(attachedTo)
{
	Colliders.push_back(this);
}

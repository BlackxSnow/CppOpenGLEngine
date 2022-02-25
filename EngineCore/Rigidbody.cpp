#include "Rigidbody.h"

COMPONENT_DEFINITION(Component, Rigidbody);

Rigidbody::Rigidbody(SceneObject* attachedTo) : Component(attachedTo)
{
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float Mass) : Component(attachedTo)
{
}

Rigidbody::Rigidbody(SceneObject* attachedTo, float Mass, PhysicsBehaviour type) : Component(attachedTo)
{

}

#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "Physics.h"
#include "Event.h"

class Collider : public Component
{
	COMPONENT_DECLARATION(Collider);

public:
	Event<Collision::CollisionData> OnCollide;


	virtual Collision::ColliderType ColliderType() = 0;
	Collider(SceneObject* attachedTo);
};
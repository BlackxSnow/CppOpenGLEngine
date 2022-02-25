#pragma once

class Collider;

namespace Collision
{
	enum class ColliderType
	{
		Sphere,
		OBB
	};

	struct CollisionData
	{
		bool AreColliding;
		float Depth;

		CollisionData(bool collision, float depth);
	};

	CollisionData Sphere2Sphere(Collider* a, Collider* b);

	extern CollisionData(*CollisionFunctions[])(Collider*, Collider*);

	CollisionData Collide(Collider* a, Collider* b);


}

namespace Physics
{
	extern float Gravity;
	extern float FixedTimestep;

	void UpdatePhysicsSimulation();

}
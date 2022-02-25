
#include "Physics.h"
#include "ETime.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "SceneObject.h"
#include "EMath.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "EngineData.h"

namespace Physics
{
	float Gravity = 9.807f;
	float FixedTimestep = 1/60;

	float AccumulatedSimTime = 0;

	void UpdatePhysicsSimulation()
	{
		AccumulatedSimTime += DeltaTime;

		while (AccumulatedSimTime > FixedTimestep)
		{
			// TODO: Do physics

			for (auto col = Colliders.begin(); col != Colliders.end(); ++col)
			{
				for (auto col2 = std::next(col); col2 != Colliders.end(); ++col2)
				{
					
					auto data = Collision::Collide(*col, *col2);
					if (data.AreColliding)
					{
						(*col)->OnCollide.Invoke(data);
					}
				}
			}

			AccumulatedSimTime -= DeltaTime;
		}
	}
}

namespace Collision
{

	CollisionData(*CollisionFunctions[])(Collider*, Collider*) =
	{
		Sphere2Sphere
	};


	CollisionData::CollisionData(bool collision, float depth)
	{
		AreColliding = collision;
		Depth = depth;
	}

	CollisionData Collision::Collide(Collider* a, Collider* b)
	{
		return CollisionFunctions[(int)a->ColliderType() + (int)b->ColliderType()](a,b);
	}

	CollisionData Collision::Sphere2Sphere(Collider* a, Collider* b)
	{
		SphereCollider* first = (SphereCollider*)a;
		SphereCollider* second = (SphereCollider*)b;

		glm::vec3 firstPos = first->GetSceneObject()->GetTransform()->GetWorldPosition() + first->Center;
		glm::vec3 secondPos = second->GetSceneObject()->GetTransform()->GetWorldPosition() + second->Center;

		

		float sqrCenterDistance = SquareDistance(firstPos, secondPos);
		float sqrRadiusDistance = pow(first->Radius, 2) + pow(second->Radius, 2);
		if (sqrCenterDistance < sqrRadiusDistance)
		{
			return CollisionData(true, sqrt(sqrCenterDistance - sqrRadiusDistance));
		}

		return CollisionData(false, 0);
	}
}


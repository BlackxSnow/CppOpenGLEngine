
#include "Physics.h"
#include "ETime.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "SceneObject.h"
#include "EMath.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "EngineData.h"
#include "AABBCollider.h"
#include "ConsoleLogging.h"
#include "EUtil.h"

#include <vector>
#include <algorithm>
#include <iomanip>

namespace Physics
{
	float Gravity = 9.807f;
	float FixedTimestep = 1.0/120.0;

	float AccumulatedSimTime = 0;

	struct RBBounds
	{
		Collision::Bounds bounds;
		Rigidbody* rigidbody;
	};

	struct
	{
		int compareAxis = 0;
		bool operator()(const RBBounds& a, const RBBounds& b)
		{
			return a.bounds.Min[compareAxis] < b.bounds.Min[compareAxis];
		}
	} _boundsCompare;

	/// <summary>
	/// Resolves a single elastic collision for cases of dynamic->static collision.
	/// </summary>
	/// <param name="target"></param>
	/// <param name="data"></param>
	void ResolveSingleElastic(const RBBounds& target, const Collision::CollisionData& data)
	{
		Transform* t = target.rigidbody->GetSceneObject()->GetTransform();
		t->SetPosition(t->GetPosition() - data.Normal * data.Depth);

		glm::vec3 normalVelocity = glm::dot(target.rigidbody->Velocity, data.Normal) * data.Normal;
		glm::vec3 tangentVelocity = target.rigidbody->Velocity - normalVelocity;

		target.rigidbody->Velocity = tangentVelocity - normalVelocity;
	}

	/// <summary>
	/// Resolve a dynamic-dynamic elastic collision.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="aData"></param>
	/// <param name="b"></param>
	/// <param name="bData"></param>
	void ResolveElastic(const RBBounds& a, const Collision::CollisionData& aData, const RBBounds& b, const Collision::CollisionData& bData)
	{
		Transform* aT = a.rigidbody->GetSceneObject()->GetTransform();
		Transform* bT = b.rigidbody->GetSceneObject()->GetTransform();

		float aSpeedAlongNormal = glm::dot(a.rigidbody->Velocity, aData.Normal);
		float bSpeedAlongNormal = glm::dot(b.rigidbody->Velocity, aData.Normal);

		glm::vec3 aVelocityAlongNormal = aSpeedAlongNormal * aData.Normal;
		glm::vec3 bVelocityAlongNormal = bSpeedAlongNormal * aData.Normal;

		float& aMass = a.rigidbody->Mass;
		float& bMass = b.rigidbody->Mass;

		// REF: https://courses.lumenlearning.com/boundless-physics/chapter/collisions/
		// "Elastic Collisions in One Dimension"
		float aFinalNormalSpeed = ((aMass - bMass) / (bMass + aMass)) * aSpeedAlongNormal + ((2 * bMass) / (bMass + aMass)) * bSpeedAlongNormal;
		float bFinalNormalSpeed = ((2 * aMass) / (bMass + aMass)) * aSpeedAlongNormal + ((bMass - aMass) / (bMass + aMass)) * bSpeedAlongNormal;

		float totalSpeed = aFinalNormalSpeed + bFinalNormalSpeed;
		float aNormalFactor;
		float bNormalFactor;

		if (totalSpeed == 0)
		{
			aNormalFactor = 0;
			bNormalFactor = 0;
		}
		else
		{
			aNormalFactor = aFinalNormalSpeed / totalSpeed;
			bNormalFactor = bFinalNormalSpeed / totalSpeed;
		}

		aT->SetPosition(aT->GetPosition() - aData.Normal * aData.Depth * aNormalFactor);
		bT->SetPosition(bT->GetPosition() - bData.Normal * bData.Depth * bNormalFactor);

		glm::vec3 aTangentVelocity = a.rigidbody->Velocity - aData.Normal * aSpeedAlongNormal;
		glm::vec3 bTangentVelocity = b.rigidbody->Velocity - aData.Normal * bSpeedAlongNormal;

		a.rigidbody->Velocity = aTangentVelocity + aData.Normal * aFinalNormalSpeed;
		b.rigidbody->Velocity = bTangentVelocity + aData.Normal * bFinalNormalSpeed;
	}


	void ResolveCollision(const RBBounds& a, const Collision::CollisionData& aData, const RBBounds& b, const Collision::CollisionData& bData)
	{
		if (a.rigidbody->PhysicsType == PhysicsBehaviour::Static)
		{
			ResolveSingleElastic(b, bData);
			return;
		}
		if (b.rigidbody->PhysicsType == PhysicsBehaviour::Static)
		{
			ResolveSingleElastic(a, aData);
			return;
		}
		ResolveElastic(a, aData, b, bData);

	}

	void CheckCollisions(RBBounds& a, RBBounds& b)
	{
		bool areColliding = false;
		Collision::CollisionData data;
		Collision::CollisionData inverseData;

		for (const auto& aCol : a.rigidbody->GetColliders())
		{
			for (const auto& bCol : b.rigidbody->GetColliders())
			{
				data = Collision::Collide(aCol, bCol);
				if (data.AreColliding)
				{
					inverseData = data.FromInverse();

					ResolveCollision(a, data, b, inverseData);

					aCol->OnCollide.Invoke(data);
					bCol->OnCollide.Invoke(inverseData);

					a.rigidbody->OnCollide.Invoke(data);
					b.rigidbody->OnCollide.Invoke(inverseData);
					areColliding = true;

					break;
				}
			}
			if (areColliding)
			{
				break;
			}
		}
	}

	void UpdatePhysicsSimulation()
	{
		AccumulatedSimTime += DeltaTime;

		while (AccumulatedSimTime >= FixedTimestep)
		{
			// TODO: Do physics
			
			std::vector<RBBounds> bounds;

			glm::vec3 centerSum = glm::vec3(0, 0, 0);
			glm::vec3 sqrCenterSum = glm::vec3(0, 0, 0);
			for (const auto& rb : Rigidbodies)
			{
				rb->Integrate(FixedTimestep);
				bounds.push_back({ rb->GetWorldBounds(), rb });
				RBBounds& rbBounds = bounds[bounds.size() - 1];
				centerSum += rbBounds.bounds.Center;
				sqrCenterSum += rbBounds.bounds.Center * rbBounds.bounds.Center;
			}

			centerSum /= bounds.size();
			sqrCenterSum /= bounds.size();
			glm::vec3 variance = sqrCenterSum - (centerSum * centerSum);

			float maxVariance = variance[0];
			int maxAxis = 0;
			for (int i = 1; i < 3; i++)
			{
				if (variance[i] > maxVariance)
				{
					maxVariance = variance[i];
					maxAxis = i;
				}
			}

			_boundsCompare.compareAxis = maxAxis;

			std::sort(bounds.begin(), bounds.end(), _boundsCompare);

			for (int i = 0; i < bounds.size(); i++)
			{
				RBBounds& a = bounds[i];
				for (int j = i + 1; j < bounds.size(); j++)
				{
					RBBounds& b = bounds[j];

					if (b.bounds.Min[maxAxis] > a.bounds.Max[maxAxis])
					{
						break;
					}

					CheckCollisions(a, b);
				}
			}

			AccumulatedSimTime -= FixedTimestep;
		}
	}
}

namespace Collision
{

	CollisionData(*CollisionFunctions[])(Collider*, Collider*) =
	{
		Sphere2Sphere, Sphere2AABB,
		AABB2Sphere, AABB2AABB
	};


	void CollisionData::Invert()
	{
		Normal = -Normal;
	}

	CollisionData CollisionData::FromInverse()
	{
		auto inverse = CollisionData(*this);
		inverse.Invert();
		return inverse;
	}

	CollisionData::CollisionData()
	{
		AreColliding = false;
		Normal = glm::vec3(0, 0, 0);
		Depth = 0;
	}

	CollisionData Collision::Collide(Collider* a, Collider* b)
	{
		int cola = (int)a->ColliderType();
		int colb = (int)b->ColliderType();
		int index = cola * ColliderCount + colb;
		return CollisionFunctions[index](a,b);
		//return CollisionFunctions[(int)a->ColliderType() * ColliderCount + (int)b->ColliderType()](a,b);
	}

	CollisionData Collision::Sphere2Sphere(Collider* a, Collider* b)
	{
		SphereCollider* first = (SphereCollider*)a;
		SphereCollider* second = (SphereCollider*)b;

		glm::vec3 firstPos = first->GetSceneObject()->GetTransform()->GetWorldPosition() + first->Center;
		glm::vec3 secondPos = second->GetSceneObject()->GetTransform()->GetWorldPosition() + second->Center;

		

		float centerDistance = glm::distance(firstPos, secondPos);
		float radiusDistance = first->Radius + second->Radius;
		if (centerDistance < radiusDistance)
		{
			CollisionData data;
			data.AreColliding = true;
			data.Depth = centerDistance - radiusDistance;
			data.Normal = glm::normalize(secondPos - firstPos);
			return data;
		}

		return CollisionData();
	}
	CollisionData Sphere2AABB(Collider* a, Collider* b)
	{
		SphereCollider* first = (SphereCollider*)a;
		AABBCollider* second = (AABBCollider*)b;

		glm::vec3 spherePos = first->GetSceneObject()->GetTransform()->GetWorldPosition() + first->Center;

		glm::vec3 secondWorldPos = second->GetSceneObject()->GetTransform()->GetWorldPosition() + second->Center;
		glm::vec3 bMin = secondWorldPos - second->Extents;
		glm::vec3 bMax = secondWorldPos + second->Extents;

		glm::vec3 closestPointToSphere = glm::vec3(
			std::clamp(spherePos.x, bMin.x, bMax.x),
			std::clamp(spherePos.y, bMin.y, bMax.y),
			std::clamp(spherePos.z, bMin.z, bMax.z)
		);

		float distance = glm::distance(spherePos, closestPointToSphere);

		bool areColliding = distance < first->Radius;

		CollisionData data;
		data.AreColliding = areColliding;

		if (areColliding)
		{
			data.Depth = first->Radius - distance;
			data.Normal = glm::normalize(closestPointToSphere - spherePos);
		}

		return data;
	}
	CollisionData AABB2Sphere(Collider* a, Collider* b)
	{
		CollisionData result = Sphere2AABB(b, a);
		result.Invert();
		return result;
	}
	CollisionData AABB2AABB(Collider* a, Collider* b)
	{
		AABBCollider* first = (AABBCollider*)a;
		AABBCollider* second = (AABBCollider*)b;

		glm::vec3 firstWorldPos = first->GetSceneObject()->GetTransform()->GetWorldPosition() + first->Center;


		glm::vec3 secondWorldPos = second->GetSceneObject()->GetTransform()->GetWorldPosition() + second->Center;


		glm::vec3 delta = secondWorldPos - firstWorldPos;
		glm::vec3 totalSize = first->Extents + second->Extents;

		bool areColliding =
			abs(delta.x) < totalSize.x &&
			abs(delta.y) < totalSize.y &&
			abs(delta.z) < totalSize.z;

		CollisionData data;
		data.AreColliding = areColliding;

		// Calculate collision data
		if (areColliding)
		{
			glm::vec3 aMin = firstWorldPos - first->Extents;
			glm::vec3 aMax = firstWorldPos + first->Extents;
			glm::vec3 bMin = secondWorldPos - second->Extents;
			glm::vec3 bMax = secondWorldPos + second->Extents;

			static const glm::vec3 normals[6] =
			{
				glm::vec3(-1,0,0), glm::vec3(1,0,0),
				glm::vec3(0,-1,0), glm::vec3(0,1,0),
				glm::vec3(0,0,-1), glm::vec3(0,0,1)
			};

			const float distances[6] =
			{
				(bMax.x - aMin.x), (aMax.x - bMin.x),
				(bMax.y - aMin.y), (aMax.y - bMin.y),
				(bMax.z - aMin.z), (aMax.z - bMin.z)
			};

			float minDepth = FLT_MAX;
			glm::vec3 minAxis;

			for (int i = 0; i < 6; i++)
			{
				if (distances[i] < minDepth)
				{
					minDepth = distances[i];
					minAxis = normals[i];
				}
			}

			data.Depth = minDepth;
			data.Normal = minAxis;
		}

		return data;
	}


	Bounds::Bounds(glm::vec3 min, glm::vec3 max)
	{
		Min = min;
		Max = max;
		glm::vec3 size = max - min;
		Extents = size / 2.0f;
		Center = (Min + Max) / 2.0f;
	}
	Bounds::Bounds()
	{
		Min = glm::vec3(0);
		Max = glm::vec3(0);
		Center = glm::vec3(0);
		Extents = glm::vec3(0);
	}
	Bounds Bounds::FromExtents(glm::vec3 center, glm::vec3 extents)
	{
		Bounds b = Bounds();
		b.Center = center;
		b.Extents = extents;
		b.Min = center - extents;
		b.Max = center + extents;
		return b;
	}
}


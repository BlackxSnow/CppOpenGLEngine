#pragma once

#include <glm/glm.hpp>

class Collider;

namespace Collision
{
	struct Bounds
	{
		glm::vec3 Min;
		glm::vec3 Max;
		glm::vec3 Center;
		glm::vec3 Extents;

		Bounds(glm::vec3 min, glm::vec3 max);
		Bounds();
		static Bounds FromExtents(glm::vec3 center, glm::vec3 extents);

	};

	const int ColliderCount = 2;
	enum class ColliderType
	{
		Sphere,
		AABB
		//OBB
	};

	struct CollisionData
	{
		bool AreColliding;

		glm::vec3 Normal;
		float Depth;

		/// <summary>
		/// Inverts the data to be from the other collider's 'perspective'.
		/// </summary>
		void Invert();
		/// <summary>
		/// Creates a new object for the inverse data.
		/// </summary>
		/// <returns></returns>
		CollisionData FromInverse();

		CollisionData();
	};

	CollisionData Sphere2Sphere(Collider* a, Collider* b);
	CollisionData Sphere2AABB(Collider* a, Collider* b);
	CollisionData AABB2Sphere(Collider* a, Collider* b);
	CollisionData AABB2AABB(Collider* a, Collider* b);

	extern CollisionData(*CollisionFunctions[])(Collider*, Collider*);

	CollisionData Collide(Collider* a, Collider* b);


}

namespace Physics
{
	extern float Gravity;
	extern float FixedTimestep;

	void UpdatePhysicsSimulation();

}
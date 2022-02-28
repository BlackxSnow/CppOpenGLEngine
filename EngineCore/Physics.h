#pragma once

#include <glm/glm.hpp>

class Collider;

namespace Collision
{
	/// <summary>
	/// Defines the limits of an axis aligned bounding box.
	/// </summary>
	struct Bounds
	{
		glm::vec3 Min;
		glm::vec3 Max;
		glm::vec3 Center;
		glm::vec3 Extents;

		Bounds(glm::vec3 min, glm::vec3 max);
		Bounds();
		/// <summary>
		/// Create a new bounds from a center and extents.
		/// </summary>
		/// <param name="center"></param>
		/// <param name="extents"></param>
		/// <returns></returns>
		static Bounds FromExtents(glm::vec3 center, glm::vec3 extents);

	};

	const int ColliderCount = 2;
	enum class ColliderType
	{
		Sphere,
		AABB
		//OBB
	};

	/// <summary>
	/// Data for collision between two colliders.
	/// </summary>
	struct CollisionData
	{
		bool AreColliding;
		/// <summary>
		/// Normal vector pointing away from impact surface.
		/// </summary>
		glm::vec3 Normal;
		/// <summary>
		/// Distance of penetration inside the collider.
		/// </summary>
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

	// Collider2Collider collision check function set.
	CollisionData Sphere2Sphere(Collider* a, Collider* b);
	CollisionData Sphere2AABB(Collider* a, Collider* b);
	CollisionData AABB2Sphere(Collider* a, Collider* b);
	CollisionData AABB2AABB(Collider* a, Collider* b);

	/// <summary>
	/// Array linking ColliderType combinations to appropriate collision functions.
	/// </summary>
	extern CollisionData(*CollisionFunctions[])(Collider*, Collider*);

	/// <summary>
	/// Calls relevant collision functions given two colliders.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	CollisionData Collide(Collider* a, Collider* b);


}

namespace Physics
{
	extern float Gravity;
	extern float FixedTimestep;

	void UpdatePhysicsSimulation();

}
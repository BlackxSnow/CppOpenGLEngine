#include "Conversion.h"

glm::vec2 cvrt::Vec2(objl::Vector2& v)
{
	return glm::vec2(v.X, v.Y);
}

glm::vec3 cvrt::Vec3(objl::Vector3& v)
{
	return glm::vec3(v.X, v.Y, v.Z);
}

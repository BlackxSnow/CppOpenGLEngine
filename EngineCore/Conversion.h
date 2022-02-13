#pragma once

#include <obj/OBJ_Loader.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace cvrt
{
	glm::vec2 Vec2(objl::Vector2& v);
	glm::vec3 Vec3(objl::Vector3& v);

}
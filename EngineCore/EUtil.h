#pragma once

#include <string>
#include <glm/glm.hpp>

inline std::string ToString(glm::vec3 input)
{
	return std::string("( " + std::to_string(input.x) + ", " + std::to_string(input.y) + ", " + std::to_string(input.z) + " )");
}
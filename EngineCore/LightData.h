#pragma once

#include <glm/vec4.hpp>

// Light types - Matches glsl definition.
const unsigned int LIGHT_POINT = 0;
const unsigned int LIGHT_DIRECTIONAL = 1;
const unsigned int LIGHT_SPOT = 2;

/// <summary>
/// Light data for openGL rendering. Matches struct within LightData.glsl for SSBO usage.
/// </summary>
struct LightData
{
	/// <summary>
	/// Relates to LightData.h constants. Type of light for rendering purposes.
	/// </summary>
	unsigned int Type;
	float Size;
	float FeatherAmount;
	int ShadowMapIndex;
	glm::vec4 Position;
	glm::vec4 Direction;
	glm::vec4 Colour;
};
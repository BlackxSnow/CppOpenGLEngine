#pragma once
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>

#include "Component.h"
#include "LightData.h"

/// <summary>
/// Base class for all light components. Provides lighting data to scene.
/// </summary>
class Light : public Component
{
	COMPONENT_DECLARATION(Light)

public:
	glm::vec4 LightColour;
	
	/// <summary>
	/// Create and return light data relevant to the type of light for use in shaders.
	/// </summary>
	/// <returns></returns>
	virtual LightData BuildLightData() = 0;

	/// <summary>
	/// Base constructor for all light components.
	/// </summary>
	/// <param name="attachedComponent"></param>
	/// <param name="lightColour">3 components for RGB value, final component represents light intensity.</param>
	Light(SceneObject* attachedComponent, glm::vec4 lightColour);


};
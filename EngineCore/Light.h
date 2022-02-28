#pragma once
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include<vector>

#include "Component.h"
#include "LightData.h"
#include "Defs.h"

/// <summary>
/// Base class for all light components. Provides lighting data to scene.
/// </summary>
class Light : public Component
{
	COMPONENT_DECLARATION(Light)

public:
	glm::vec4 LightColour;
	/// <summary>
	/// Whether or not this light contributes to real time shadows.
	/// </summary>
	bool isShadowCaster = true;
	
	/// <summary>
	/// Create and return light data relevant to the type of light for use in shaders.
	/// </summary>
	/// <returns></returns>
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) = 0;

	/// <summary>
	/// Construct view and projection matrices for the light's shadow rendering within the provided pointers.
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) = 0;

	/// <summary>
	/// Base constructor for all light components.
	/// </summary>
	/// <param name="attachedComponent"></param>
	/// <param name="lightColour">3 components for RGB value, final component represents light intensity.</param>
	Light(SceneObject* attachedComponent, glm::vec4 lightColour);

	~Light();
};
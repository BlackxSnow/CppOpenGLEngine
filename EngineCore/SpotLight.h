#pragma once
#include "Light.h"

/// <summary>
/// Conical light with no loss in luminosity over distance.
/// </summary>
class SpotLight : public Light
{
	COMPONENT_DECLARATION(SpotLight)

public:
	/// <summary>
	/// The angle of the light's cone in degrees.
	/// </summary>
	float Size;
	/// <summary>
	/// Additional angle of luminosity feathering outside of Size.
	/// </summary>
	float FeatherAmount;
	
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) override;
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) override;

	SpotLight(SceneObject* attachedComponent, glm::vec4 lightColour, float size, float featherAmount);

private:
	
};
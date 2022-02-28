#pragma once
#include "Light.h"

/// <summary>
/// 'Infinite' distance light with static luminosity and parallel 'rays'.
/// </summary>
class DirectionalLight : public Light
{
	COMPONENT_DECLARATION(DirectionalLight)

public:
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) override;
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) override;

	DirectionalLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction);

private:
	glm::vec3 Direction;
};
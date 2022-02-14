#pragma once
#include "Light.h"

class PointLight : public Light
{
	COMPONENT_DECLARATION(PointLight)

public:
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) override;
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) override;

	PointLight(SceneObject* attachedComponent, glm::vec4 lightColour);
};
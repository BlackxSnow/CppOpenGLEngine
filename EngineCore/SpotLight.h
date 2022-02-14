#pragma once
#include "Light.h"

class SpotLight : public Light
{
	COMPONENT_DECLARATION(SpotLight)

public:
	float Size;
	float FeatherAmount;
	
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) override;
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) override;

	SpotLight(SceneObject* attachedComponent, glm::vec4 lightColour, float size, float featherAmount);

private:
	
};
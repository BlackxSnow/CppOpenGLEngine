#pragma once
#include "Light.h"

class SpotLight : public Light
{
	COMPONENT_DECLARATION(SpotLight)

public:
	glm::vec3 Direction;
	float Size;
	float FeatherAmount;
	
	virtual LightData BuildLightData() override;

	SpotLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction, float size, float featherAmount);

private:
	
};
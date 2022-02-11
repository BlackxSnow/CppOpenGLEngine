#pragma once
#include "Light.h"

class PointLight : public Light
{
	COMPONENT_DECLARATION(PointLight)

public:
	virtual LightData BuildLightData() override;

	PointLight(SceneObject* attachedComponent, glm::vec4 lightColour);
};
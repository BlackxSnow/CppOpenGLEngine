#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
	COMPONENT_DECLARATION(DirectionalLight)

public:
	//virtual glm::vec3 GetLightDirection() override;
	virtual LightData BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices) override;
	virtual void BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection) override;

	DirectionalLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction);

private:
	glm::vec3 Direction;
};
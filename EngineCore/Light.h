#pragma once
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>

#include "Component.h"
#include "LightData.h"

class Light : public Component
{
	COMPONENT_DECLARATION(Light)

public:
	glm::vec4 LightColour;
	
	//virtual glm::vec3 GetLightDirection() = 0;
	virtual LightData BuildLightData() = 0;

	Light(SceneObject* attachedComponent, glm::vec4 lightColour);


};
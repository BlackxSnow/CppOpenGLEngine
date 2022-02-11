#include "Light.h"

COMPONENT_DEFINITION(Component, Light)

Light::Light(SceneObject* attachedComponent, glm::vec4 lightColour) : Component(attachedComponent) 
{
	LightColour = lightColour;
}
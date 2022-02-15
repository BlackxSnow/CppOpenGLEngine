#include "Light.h"
#include "EngineData.h"

COMPONENT_DEFINITION(Component, Light)

void Register(Light* light)
{
	SceneLights.push_back(light);
}

void Deregister(Light* light)
{
	SceneLights.remove(light);
}

Light::Light(SceneObject* attachedComponent, glm::vec4 lightColour) : Component(attachedComponent) 
{
	LightColour = lightColour;
	Register(this);
}

Light::~Light()
{
	Deregister(this);
}

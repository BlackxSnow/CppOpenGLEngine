#include "PointLight.h"
#include "SceneObject.h"

COMPONENT_DEFINITION(Light, PointLight)

LightData PointLight::BuildLightData()
{
    LightData result = LightData();

    result.Type = LIGHT_POINT;
    result.Position = glm::vec4(GetSceneObject()->GetTransform()->GetWorldPosition(), 1);
    result.Direction = glm::vec4(0);
    result.Colour = LightColour;
    result.Size = 0;

    return result;
}

PointLight::PointLight(SceneObject* attachedComponent, glm::vec4 lightColour) : Light(attachedComponent, lightColour)
{
}

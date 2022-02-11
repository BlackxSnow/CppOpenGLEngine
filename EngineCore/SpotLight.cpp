#include "SpotLight.h"
#include "SceneObject.h"

COMPONENT_DEFINITION(Light, SpotLight)

LightData SpotLight::BuildLightData()
{
    LightData result = LightData();
    result.Type = LIGHT_SPOT;
    result.Direction = glm::vec4(GetSceneObject()->GetTransform()->GetWorldRotation() * Direction, 1);
    float sizeRadians = glm::radians(Size);
    result.Size = cos(sizeRadians);
    result.FeatherAmount = cos(sizeRadians + glm::radians(FeatherAmount));
    result.Position = glm::vec4(GetSceneObject()->GetTransform()->GetWorldPosition(), 1);
    result.Colour = LightColour;

    return result;
}

SpotLight::SpotLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction, float size, float featherAmount) : Light(attachedComponent, lightColour)
{
    Direction = direction;
    Size = size;
    FeatherAmount = featherAmount;
}

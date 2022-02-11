#include <glm/geometric.hpp>

#include "DirectionalLight.h"
#include "SceneObject.h"

COMPONENT_DEFINITION(Light, DirectionalLight)

//glm::vec3 DirectionalLight::GetLightDirection()
//{
//    return Direction;
//}

LightData DirectionalLight::BuildLightData()
{
    LightData result = LightData();

    result.Type = LIGHT_DIRECTIONAL;
    result.Position = glm::vec4(0, 0, 0, 0);
    result.Direction = glm::vec4(Direction, 1);
    result.Colour = LightColour;
    result.Size = 0;

    return result;
}

DirectionalLight::DirectionalLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction) : Light(attachedComponent, lightColour)
{
    Direction = glm::normalize(direction);
}

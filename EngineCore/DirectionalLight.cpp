#include <glm/geometric.hpp>

#include "DirectionalLight.h"
#include "SceneObject.h"

COMPONENT_DEFINITION(Light, DirectionalLight)

//glm::vec3 DirectionalLight::GetLightDirection()
//{
//    return Direction;
//}

LightData DirectionalLight::BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices)
{
    LightData result = LightData();

    result.Type = LIGHT_DIRECTIONAL;
    result.Position = glm::vec4(0, 0, 0, 0);
    result.Direction = glm::vec4(Direction, 1);
    result.Colour = LightColour;
    result.Size = 0;

    if (isShadowCaster)
    {
        glm::mat4 view, proj, lightMatrix;
        BuildLightMatrices(&view, &proj);
        lightMatrix = proj * view;
        lightMatrices.push_back(lightMatrix);
        result.ShadowMapIndex = *shadowMapIndex;
        (*shadowMapIndex)++;
    }

    return result;
}

void DirectionalLight::BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection)
{
    //TODO
}

DirectionalLight::DirectionalLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction) : Light(attachedComponent, lightColour)
{
    Direction = glm::normalize(direction);
}

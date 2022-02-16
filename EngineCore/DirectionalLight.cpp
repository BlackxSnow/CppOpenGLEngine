#include <glm/geometric.hpp>

#include "DirectionalLight.h"
#include "SceneObject.h"
#include "EngineData.h"

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
    const float size = 10;

    glm::vec3 camPos = ActiveCamera->GetSceneObject()->GetTransform()->GetWorldPosition();
    glm::vec3 falseLightPos = camPos + -Direction * 50.0f;
    glm::vec3 up = glm::dot(glm::vec3(0, 1, 0), Direction) != 0 ? glm::vec3(0, 1, 0) : glm::vec3(0, 1, -1);
    *view = glm::lookAt(falseLightPos, falseLightPos + Direction, up);
    *projection = glm::ortho(-size, size, -size, size, 0.1f, 100.0f);
}

DirectionalLight::DirectionalLight(SceneObject* attachedComponent, glm::vec4 lightColour, glm::vec3 direction) : Light(attachedComponent, lightColour)
{
    Direction = glm::normalize(direction);
}

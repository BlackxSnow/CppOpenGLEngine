#include "PointLight.h"
#include "SceneObject.h"

COMPONENT_DEFINITION(Light, PointLight)

LightData PointLight::BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices)
{
    LightData result = LightData();

    result.Type = LIGHT_POINT;
    result.Position = glm::vec4(GetSceneObject()->GetTransform()->GetWorldPosition(), 1);
    result.Direction = glm::vec4(0);
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

void PointLight::BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection)
{
    //TODO
}

PointLight::PointLight(SceneObject* attachedComponent, glm::vec4 lightColour) : Light(attachedComponent, lightColour)
{
}

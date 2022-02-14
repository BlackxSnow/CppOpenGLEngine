#include "SpotLight.h"
#include "SceneObject.h"
#include <glm/gtx/quaternion.hpp>

COMPONENT_DEFINITION(Light, SpotLight)

LightData SpotLight::BuildLightData(int* shadowMapIndex, std::vector<glm::mat4>& lightMatrices)
{
    LightData result = LightData();
    result.Type = LIGHT_SPOT;
    result.Direction = glm::vec4(GetSceneObject()->GetTransform()->GetWorldRotation() * glm::vec3(0,0,-1), 1);
    float sizeRadians = glm::radians(Size);
    result.Size = cos(sizeRadians);
    result.FeatherAmount = cos(sizeRadians + glm::radians(FeatherAmount));
    result.Position = glm::vec4(GetSceneObject()->GetTransform()->GetWorldPosition(), 1);
    result.Colour = LightColour;

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

void SpotLight::BuildLightMatrices(OUT glm::mat4* view, OUT glm::mat4* projection)
{
    glm::vec3 lightPosition = GetSceneObject()->GetTransform()->GetWorldPosition();

    *view = glm::translate(glm::mat4(1), lightPosition) * glm::toMat4(GetSceneObject()->GetTransform()->GetWorldRotation());
    *view = glm::inverse(*view);
    *projection = glm::perspective(glm::radians(Size + FeatherAmount) * 2, (float)(windowWidth / windowHeight), 0.1f, 100.0f);
}

SpotLight::SpotLight(SceneObject* attachedComponent, glm::vec4 lightColour, float size, float featherAmount) : Light(attachedComponent, lightColour)
{
    Size = size;
    FeatherAmount = featherAmount;
}

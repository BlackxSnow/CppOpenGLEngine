#pragma once

#include <vector>
#include<GL/glew.h>

#include "SceneObject.h"
#include "Transform.h"
#include "Light.h"



class Camera;

extern GLuint ShadowMapArray;

extern std::shared_ptr<Camera> ActiveCamera;
extern std::vector<std::shared_ptr<Light>> SceneLights;

extern std::shared_ptr<std::vector<glm::mat4>> ShadowCasterMatrices;
#pragma once

#include <vector>

#include "SceneObject.h"
#include "Transform.h"
#include "Light.h"



class Camera;
std::shared_ptr<Camera> ActiveCamera;
std::vector<std::shared_ptr<Light>> SceneLights;
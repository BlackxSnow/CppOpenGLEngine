#pragma once

#include <list>
#include <GL/glew.h>
#include <map>

#include "SceneObject.h"
#include "Renderer.h"
#include "Light.h"
#include "Camera.h"
#include "Callbacks.h"

extern GLuint ShadowMapArray;

extern std::list<SceneObject*> SceneObjects;

extern std::list<Renderer*> Renderers;
extern std::list<Light*> SceneLights;

extern Camera* ActiveCamera;


extern std::shared_ptr<std::vector<glm::mat4>> ShadowCasterMatrices;

// Callbacks
extern std::list<IUpdatable*> Updatable;
extern std::list<ILateUpdatable*> LateUpdatable;
extern std::list<IFixedUpdatable*> FixedUpdatable;
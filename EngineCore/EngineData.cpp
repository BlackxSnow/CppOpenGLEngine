#include "EngineData.h"

GLuint ShadowMapArray = -1;

std::list<SceneObject*> SceneObjects;

std::list<Renderer*> Renderers;
std::list<Light*> SceneLights;

Camera* ActiveCamera;


std::shared_ptr<std::vector<glm::mat4>> ShadowCasterMatrices;

// Callbacks
std::list<IUpdatable*> Updatable;
std::list<ILateUpdatable*> LateUpdatable;
std::list<IFixedUpdatable*> FixedUpdatable;
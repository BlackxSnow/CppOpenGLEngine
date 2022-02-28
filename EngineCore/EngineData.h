#pragma once

#include <list>
#include <GL/glew.h>
#include <map>

#include "SceneObject.h"
#include "Renderer.h"
#include "Light.h"
#include "Camera.h"
#include "Callbacks.h"
#include "Collider.h"
#include "Rigidbody.h"

/// <summary>
/// ID of the shadowMapArray openGL texture object.
/// </summary>
extern GLuint ShadowMapArray;

/// <summary>
/// All objects within the world.
/// </summary>
extern std::list<SceneObject*> SceneObjects;

/// <summary>
/// All active renderers in the world.
/// </summary>
extern std::list<Renderer*> Renderers;
/// <summary>
/// All active lights within the world.
/// </summary>
extern std::list<Light*> SceneLights;

/// <summary>
/// All active rigidbodies.
/// </summary>
extern std::list<Rigidbody*> Rigidbodies;
/// <summary>
/// All active colliders without rigidbodies.
/// </summary>
extern std::list<Collider*> Colliders;

/// <summary>
/// Currently active camera.
/// </summary>
extern Camera* ActiveCamera;

/// <summary>
/// Combined matrices for shadow casting lights.
/// </summary>
extern std::shared_ptr<std::vector<glm::mat4>> ShadowCasterMatrices;

// Callbacks
extern std::list<IUpdatable*> Updatable;
extern std::list<ILateUpdatable*> LateUpdatable;
extern std::list<IFixedUpdatable*> FixedUpdatable;
#pragma once


#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SceneObject.h"
#include "Transform.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Event.h"

/// <summary>
/// Initialise engine values and objects.
/// </summary>
/// <returns></returns>
int InitialiseEngine();

/// <summary>
/// Begin the engine game loop. Returns on exit.
/// </summary>
/// <returns></returns>
int StartEngineLoop();

/// <summary>
/// Hash map of shaders by their names.
/// </summary>
extern std::map<std::string, std::shared_ptr<Shader>> Shaders;
/// <summary>
/// Reference to the GLFW window object.
/// </summary>
extern GLFWwindow* Window;

/// <summary>
/// Invoked after world rendering is completed for screen space UI.
/// </summary>
extern Event<> OnGUIDraw;

/// <summary>
/// Set the provided camera as the currently active renderer.
/// </summary>
void SetActiveCamera(std::shared_ptr<Camera> cam);
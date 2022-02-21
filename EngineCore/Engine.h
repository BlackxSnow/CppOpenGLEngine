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

int InitialiseEngine();
int StartEngineLoop();

extern std::map<std::string, std::shared_ptr<Shader>> Shaders;
extern GLFWwindow* Window;

extern Event<> OnGUIDraw;

void SetActiveCamera(std::shared_ptr<Camera> cam);
#include "Engine.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>


#include "Renderer.h"
#include "CameraController.h"
#include "SpotLight.h"

int main()
{
	InitialiseEngine();

	SceneObject* camera = new SceneObject(glm::vec3(0, 0, 1), glm::quat());
	auto cameraComponent = camera->AddComponent<Camera>(75, 0.1f, 100.0f);
	camera->AddComponent<CameraController>(camera->GetTransform());

	SetActiveCamera(cameraComponent);

	SceneObject* plane = new SceneObject(glm::vec3(0, -1, -1), glm::quat());
	plane->GetTransform()->SetScale(glm::vec3(10, 1, 10));
	auto planeRenderer = plane->AddComponent<Renderer>(Shaders["default"]);
	planeRenderer->ImportMeshesFromOBJ("Models/Plane.obj");

	SceneObject* cube = new SceneObject(glm::vec3(0, 0, -2), glm::quat());
	auto cubeRenderer = cube->AddComponent<Renderer>(Shaders["default"]);
	cubeRenderer->ImportMeshesFromOBJ("Models/Cube.obj");

	
	SceneObject* light = new SceneObject(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	auto lightComponent = light->AddComponent<SpotLight>(glm::vec4(1, 1, 1, 1), 15, 15);
	
	StartEngineLoop();

	return 0;
}
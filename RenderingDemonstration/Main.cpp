#include "Engine.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>


#include "Renderer.h"
#include "CameraController.h"
#include "SpotLight.h"
#include <DirectionalLight.h>
#include "TextRendering.h"
#include "ConsoleLogging.h"
#include "SphereCollider.h"
#include "Rigidbody.h"

void STDOUT(Collision::CollisionData d)
{
	std::cout << "Colliding! Depth: " << d.Depth << std::endl;
}

int main()
{
	InitialiseEngine();

	txt::LoadFont(std::filesystem::absolute(std::filesystem::path(".\\Fonts\\Metropolis-Regular.otf")).string());

	OnGUIDraw.Register("DrawText", []() { txt::RenderText("Text Test", "Metropolis-Regular", 25, 25, 1, glm::vec3(1, 1, 1)); });

	SceneObject* camera = new SceneObject(glm::vec3(0, 0, 1), glm::vec3(0,0,-1), glm::vec3(0,1,0));
	auto cameraComponent = camera->AddComponent<Camera>(75, 0.1f, 100.0f);
	camera->AddComponent<CameraController>(camera->GetTransform());

	SetActiveCamera(cameraComponent);

	SceneObject* plane = new SceneObject(glm::vec3(0, -1, -1), glm::quat());
	plane->GetTransform()->SetScale(glm::vec3(10, 1, 10));
	auto planeRenderer = plane->AddComponent<Renderer>(Shaders["default"]);
	planeRenderer->ImportMeshesFromOBJ("Models/Plane.obj");

	SceneObject* cube = new SceneObject(glm::vec3(0, 0, -2), glm::quat());
	auto cubeRenderer = cube->AddComponent<Renderer>(Shaders["default"]);
	cube->AddComponent<Rigidbody>();
	cube->AddComponent<SphereCollider>(1.0f);
	cubeRenderer->ImportMeshesFromOBJ("Models/Cube.obj");

	//SceneObject* cube2 = new SceneObject(glm::vec3(0, 2.1f, -1.5f), glm::quat());
	//auto cubeRenderer2 = cube2->AddComponent<Renderer>(Shaders["default"]);
	//cube2->AddComponent<SphereCollider>(1.0f)->OnCollide.Register("STDOUT", std::function<void(Collision::CollisionData)>(STDOUT));
	//cubeRenderer2->ImportMeshesFromOBJ("Models/Cube.obj");

	
	SceneObject* light = new SceneObject(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	//auto lightComponent = light->AddComponent<SpotLight>(glm::vec4(1, 1, 1, 1), 15, 15);
	auto lightComponent = light->AddComponent<DirectionalLight>(glm::vec4(1, 1, 1, 1), glm::normalize(glm::vec3(-1, -1, 1)));

	StartEngineLoop();

	return 0;
}
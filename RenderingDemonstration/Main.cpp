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
#include "AABBCollider.h"
#include "Rigidbody.h"
#include "InputManager.h"

/// <summary>
/// Create a cube object with pre-defined components.
/// </summary>
/// <param name="pos"></param>
/// <param name="mass"></param>
/// <param name="colour"></param>
SceneObject* CreateCube(glm::vec3 pos, float mass = 1, glm::vec3 colour = glm::vec3(0.7f,0.7f,0.7f))
{
	SceneObject* cube = new SceneObject(pos, glm::quat());
	auto cubeRenderer = cube->AddComponent<Renderer>(Shaders["default"]);
	cubeRenderer->ColourSetting = ShaderColour::Uniform;
	cubeRenderer->Colour = colour;
	cube->AddComponent<Rigidbody>(mass);
	cube->AddComponent<AABBCollider>(1.0f);
	cubeRenderer->ImportMeshesFromOBJ("Models/Cube.obj");
	return cube;
}

int main()
{
	InitialiseEngine();

	txt::LoadFont(std::filesystem::absolute(std::filesystem::path(".\\Fonts\\Metropolis-Regular.otf")).string());

	OnGUIDraw.Register("DrawText", []() { txt::RenderText("Text Test", "Metropolis-Regular", 25, 25, 1, glm::vec3(1, 1, 1)); });

	SceneObject* camera = new SceneObject(glm::vec3(0, 1, 15), glm::vec3(0,0,-1), glm::vec3(0,1,0));
	auto cameraComponent = camera->AddComponent<Camera>(75, 0.1f, 100.0f);
	camera->AddComponent<CameraController>(camera->GetTransform());

	SetActiveCamera(cameraComponent);

	SceneObject* plane = new SceneObject(glm::vec3(0, -1, -1), glm::quat());
	plane->GetTransform()->SetScale(glm::vec3(10, 1, 10));
	auto planeRenderer = plane->AddComponent<Renderer>(Shaders["default"]);
	planeRenderer->ColourSetting = ShaderColour::Uniform;
	planeRenderer->Colour = glm::vec3(0.7f, 0.7f, 0.7f);
	plane->AddComponent<Rigidbody>(1, PhysicsBehaviour::Static);
	plane->AddComponent<AABBCollider>(glm::vec3(0,-1,0), glm::vec3(10,1.0f, 10));
	planeRenderer->ImportMeshesFromOBJ("Models/Plane.obj");

	auto blueCube = CreateCube(glm::vec3(0, 5, -2), 1, glm::vec3(1,0,0));
	blueCube->Name = "Blue Cube";
	auto greenCube = CreateCube(glm::vec3(0, 10, -2), 1, glm::vec3(0,1,0));
	greenCube->Name = "Green Cube";

	
	SceneObject* light = new SceneObject(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	//auto lightComponent = light->AddComponent<SpotLight>(glm::vec4(1, 1, 1, 1), 15, 15);
	auto lightComponent = light->AddComponent<DirectionalLight>(glm::vec4(1, 1, 1, 1), glm::normalize(glm::vec3(-1, -1, 1)));

	RegisterInputCallback(GLFW_KEY_SPACE, GLFW_PRESS, "STDCOUTLINE", std::function<void(int)>([](int _) {std::cout << "----------\n"; }));

	StartEngineLoop();

	return 0;
}
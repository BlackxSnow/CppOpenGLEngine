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
SceneObject* CreateCube(glm::vec3 pos, float mass = 1, glm::vec3 colour = glm::vec3(0.7f,0.7f,0.7f), glm::vec3 velocity = glm::vec3(0,0,0), bool useGravity = true)
{
	SceneObject* cube = new SceneObject(pos, glm::quat());
	auto cubeRenderer = cube->AddComponent<Renderer>(Shaders["default"]);
	cubeRenderer->ColourSetting = ShaderColour::Uniform;
	cubeRenderer->Colour = colour;
	auto rb = cube->AddComponent<Rigidbody>(mass);
	rb->Velocity = velocity;
	rb->UseGravity = useGravity;
	cube->AddComponent<AABBCollider>(1.0f);
	cubeRenderer->ImportMeshesFromOBJ("Models/Cube.obj");
	return cube;
}

SceneObject* CreateWall(glm::vec3 pos, glm::vec3 aabbExtents)
{
	SceneObject* wall = new SceneObject(pos, glm::quat());
	auto rb = wall->AddComponent<Rigidbody>(1);
	rb->PhysicsType = PhysicsBehaviour::Static;
	wall->AddComponent<AABBCollider>(glm::vec3(0, 0, 0), aabbExtents);
	return wall;
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

	CreateWall(glm::vec3(-10, 10, 0), glm::vec3(0.5f, 10, 10));
	CreateWall(glm::vec3(10, 10, 0), glm::vec3(0.5f, 10, 10));
	CreateWall(glm::vec3(0, 10, -10), glm::vec3(10, 10, 0.5f));
	CreateWall(glm::vec3(0, 10, 10), glm::vec3(10, 10, 0.5f));

	auto redCube = CreateCube(glm::vec3(0, 5, 0), 1, glm::vec3(1,0,0));
	redCube->Name = "Red Cube";
	auto greenCube = CreateCube(glm::vec3(0, 10, 0), 1, glm::vec3(0,1,0));
	greenCube->Name = "Green Cube";

	auto blueCube = CreateCube(glm::vec3(9, 5, 0), 1, glm::vec3(0, 0, 1), glm::vec3(-10, 0, 0), false);

	
	SceneObject* light = new SceneObject(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	//auto lightComponent = light->AddComponent<SpotLight>(glm::vec4(1, 1, 1, 1), 15, 15);
	auto lightComponent = light->AddComponent<DirectionalLight>(glm::vec4(1, 1, 1, 1), glm::normalize(glm::vec3(-1, -1, 1)));

	StartEngineLoop();

	return 0;
}
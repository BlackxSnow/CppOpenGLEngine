#include "Main.h"

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Defs.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Mesh.h"


// Vertices coordinates
std::vector<Vertex> vertices
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex(glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f))
};

// Indices for vertices order
std::vector<GLuint> indices
{
	0, 1, 2,
	0, 2, 3
};

/// <summary>
/// Initialise a glfw window to the provided pointer.
/// </summary>
/// <param name="window"></param>
/// <returns></returns>
int InitWindow(OUT GLFWwindow** window)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLTest", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(*window);
	return 0;
}

std::shared_ptr<SceneObject> MakeLightObject(OUT std::shared_ptr<SpotLight>* lightComponent)
{
	//std::shared_ptr<SceneObject> lightObj = std::shared_ptr<SceneObject>(new SceneObject());
	//*lightComponent = lightObj->AddComponent<DirectionalLight>(glm::vec4(1.0f, 0.2f, 0.2f, 1.0f), glm::vec3(-1.0f, 0, -1.0f));

	//std::shared_ptr<SceneObject> lightObj = std::shared_ptr<SceneObject>(new SceneObject(glm::vec3(0, 1.0f, -1), glm::quat()));
	//*lightComponent = lightObj->AddComponent<PointLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	std::shared_ptr<SceneObject> lightObj = std::shared_ptr<SceneObject>(new SceneObject(glm::vec3(0, 1.0f, -1), glm::quat()));
	*lightComponent = lightObj->AddComponent<SpotLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0, -1, 0, 0), 15, 15);

	return lightObj;
}

void LoadIncludes()
{
	// TODO find all .glsl files in a folder and load automatically

	const char* name = "/LightData";
	std::ifstream ldFile("LightData.glsl", std::ios::ate | std::ios::in | std::ios::binary);
	ldFile.seekg(0, std::ios::end);
	std::streampos size = ldFile.tellg();
	std::streampos dataLength = size;
	dataLength += 1;

	char* data = new char[dataLength];
	data[size] = '\0';
	ldFile.seekg(0, std::ios::beg);
	ldFile.read(data, size);
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(name), name, dataLength, data);

	delete[] data;
}

GLuint GenerateLightBuffer()
{
	GLuint lightSSBO;
	glGenBuffers(1, &lightSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(LightData) * 10, NULL, GL_DYNAMIC_DRAW);
	return lightSSBO;
}

std::vector<LightData> lights;
std::shared_ptr<SpotLight> lightComponent;

void SetLightBufferData(Shader& shader, Camera& camera)
{
	lights.clear();
	lights.push_back(lightComponent->BuildLightData());
	int lightCount = lights.size();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &lightCount);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(LightData) * lights.size(), &lights[0]);
}

void SetLightBufferData()
{
	lights.clear();
	lights.push_back(lightComponent->BuildLightData());
	int lightCount = lights.size();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &lightCount);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(LightData) * lights.size(), &lights[0]);
}

#include "ConsoleLogging.h"
#include "Renderer.h"
int main()
{
	GLFWwindow* window;
	InitWindow(&window);

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	LoadIncludes();
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);

	std::vector<std::shared_ptr<Texture>> textures
	{
		std::shared_ptr<Texture>(new Texture("brick.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D))
	};

	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader("default.vert", "default.frag"));

	SceneObject* camObject = new SceneObject();
	ActiveCamera = camObject->AddComponent<Camera>(90, 0.1f, 100.0f);

	camObject->GetTransform()->Translate(glm::vec3(0.0, 0.5f, 1.0f));

	auto lightObject = MakeLightObject(OUT &lightComponent);

	std::shared_ptr<SceneObject> floor(new SceneObject(glm::vec3(0, -1, -1), glm::quat()));
	std::vector<std::shared_ptr<Mesh>> floorMeshes;
	floorMeshes.emplace_back(std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures)));
	std::shared_ptr<Renderer> floorRenderer = floor->AddComponent<Renderer>(shader, floorMeshes);
	//floorMesh->OnRender.Register(SetLightBufferData);

	std::shared_ptr<SceneObject> cube(new SceneObject(glm::vec3(0, -1, -2.25), glm::quat()));
	std::shared_ptr<Renderer> cubeRenderer = cube->AddComponent<Renderer>(shader);
	cubeRenderer->ImportMeshesFromOBJ("Cube.obj");

	GenerateLightBuffer();

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);

	float deltaTime = 0;
	double lastTime = glfwGetTime();

	glm::mat4 model(1);

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			ActiveCamera->GetSceneObject()->GetTransform()->Translate(glm::vec3(-1 * deltaTime, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			ActiveCamera->GetSceneObject()->GetTransform()->Translate(glm::vec3(1 * deltaTime, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			ActiveCamera->GetSceneObject()->GetTransform()->Translate(glm::vec3(0, 0, -1 * deltaTime));
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			ActiveCamera->GetSceneObject()->GetTransform()->Translate(glm::vec3(0, 0, 1 * deltaTime));
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			lightObject->GetTransform()->Translate(glm::vec3(-1 * deltaTime, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			lightObject->GetTransform()->Translate(glm::vec3(1 * deltaTime, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			lightObject->GetTransform()->Translate(glm::vec3(0, 0, -1 * deltaTime));
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			lightObject->GetTransform()->Translate(glm::vec3(0, 0, 1 * deltaTime));
		}

		SetLightBufferData();
		floorRenderer->Draw(*ActiveCamera);
		cubeRenderer->Draw(*ActiveCamera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader->Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#include "Main.h"

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/type_ptr.hpp>
#include <map>

#include "Shader.h"
#include "Defs.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ConsoleLogging.h"
#include "Renderer.h"
#include "GLErrors.h"

#include "Mesh.h"


std::shared_ptr<Camera> ActiveCamera;
std::vector<std::shared_ptr<Light>> SceneLights;

std::shared_ptr<std::vector<glm::mat4>> ShadowCasterMatrices;

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
	2, 1, 0,
	3, 2, 0
};

std::vector<std::shared_ptr<Renderer>> Renderers;
std::map<std::string, std::shared_ptr<Shader>> Shaders;

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

	std::shared_ptr<SceneObject> lightObj = std::shared_ptr<SceneObject>(new SceneObject(glm::vec3(0, 1.0f, -1), glm::vec3(0, -1, 0), glm::vec3(0,0,-1)));
	*lightComponent = lightObj->AddComponent<SpotLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 15, 15);

	SceneLights.push_back((std::shared_ptr<Light>)*lightComponent);

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

std::vector<LightData> lightData;
std::shared_ptr<SpotLight> lightComponent;

GLuint ShadowMapFrameBuffer;
GLuint ShadowMapArray = -1;
int LastShadowMapCount = 0;
const int LightMapResolution = 1024;

GLuint DEBUGSHADOW;

void RenderLightShadowMaps(int mapCount)
{
	
	if (LastShadowMapCount != mapCount)
	{
		// Generate new texture array with correct depth.
		if (ShadowMapArray != -1)
		{
			glDeleteTextures(1, &ShadowMapArray);
		}
		glGenTextures(1, &ShadowMapArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, ShadowMapArray);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16, LightMapResolution, LightMapResolution, mapCount, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		LastShadowMapCount = mapCount;
	}

	gler::ProcessGLErrors(CLOGINFO, true);
	glEnable(GL_DEPTH_TEST);

	// Configure the frame buffer and viewport.
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFrameBuffer);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, LightMapResolution, LightMapResolution);
	

	std::shared_ptr<Shader> mapShader = Shaders["shadowMap"];
	gler::ProcessGLErrors(CLOGINFO, false);

	mapShader->Activate();

	//glDeleteTextures(1, &DEBUGSHADOW);
	//glGenTextures(1, &DEBUGSHADOW);
	//glBindTexture(GL_TEXTURE_2D, DEBUGSHADOW);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, LightMapResolution, LightMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int currentIndex = 0;
	for (std::shared_ptr<Light> light : SceneLights)
	{
		if (light->isShadowCaster == false)
		{
			continue;
		}
		// Assign the current layer of the ShadowMapArray as the depth render target.
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ShadowMapArray, 0, currentIndex);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DEBUGSHADOW, 0);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		gler::ProcessGLErrors(CLOGINFO, false);

		glm::mat4 lightView;
		glm::mat4 lightProj;
		light->BuildLightMatrices(&lightView, &lightProj);
		glm::mat4 lightMatrix = lightProj * lightView;

		glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightMatrix));

		gler::ProcessGLErrors(CLOGINFO);

		for (std::shared_ptr<Renderer> renderer : Renderers)
		{
			glCullFace(renderer->ShadowMapCullingMode);
			renderer->Draw(*mapShader, true);
		}

		gler::ProcessGLErrors(CLOGINFO);
		currentIndex++;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		clog::Error(CLOGINFO, "Framebuffer was not complete", false);
	}

	gler::ProcessGLErrors(CLOGINFO);

	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);

	gler::ProcessGLErrors(CLOGINFO);
}

void SetLightBufferData()
{
	lightData.clear();
	ShadowCasterMatrices->clear();
	int currentShadowMapIndex = 0;
	for (std::shared_ptr<Light> light : SceneLights)
	{
		lightData.push_back(light->BuildLightData(&currentShadowMapIndex, *ShadowCasterMatrices));
	}

	int lightCount = lightData.size();
	if (lightCount == 0)
	{
		return;
	}
	
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &lightCount);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(LightData) * lightData.size(), &lightData[0]);

	RenderLightShadowMaps(currentShadowMapIndex);
}

void RenderTextureToScreen(GLuint texID, GLenum texType)
{
	std::vector<GLfloat> quadVerts
	{
		-1, -1, 0, 0,
		1, -1, 1, 0,
		1, 1, 1, 1,
		-1, 1, 0, 1
	};
	std::vector<GLuint> quadIndices
	{
		0, 1, 2,
		2, 3, 0
	};

	auto screenRender = Shaders["screenRender"];

	glDisable(GL_DEPTH_TEST);
	screenRender->Activate();

	VertexArray VAO;
	VAO.Bind();

	GLuint VBO;
	GLuint EBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, quadVerts.size() * sizeof(GLfloat), quadVerts.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(GLuint), quadIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(texType, texID);
	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(screenRender->ID, "screenTexture"), 0);

	glDrawElements(GL_TRIANGLES, quadIndices.size(), GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

	gler::ProcessGLErrors(CLOGINFO, false);
}

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
	glEnable(GL_CULL_FACE);

	glGenFramebuffers(1, &ShadowMapFrameBuffer);
	ShadowCasterMatrices = std::shared_ptr<std::vector<glm::mat4>>(new std::vector<glm::mat4>);

	std::vector<std::shared_ptr<Texture>> textures
	{
		std::shared_ptr<Texture>(new Texture("brick.png", "diffuse", GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D))
	};

	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader("default.vert", "default.frag"));
	std::shared_ptr<Shader> shadowMapShader = std::shared_ptr<Shader>(new Shader("ShadowMap.vert", "ShadowMap.frag"));
	std::shared_ptr<Shader> screenRender = std::shared_ptr<Shader>(new Shader("ScreenRender.vert", "ScreenRender.frag"));

	Shaders.insert({"default", shader});
	Shaders.insert({ "shadowMap", shadowMapShader });
	Shaders.insert({ "screenRender", screenRender });

	SceneObject* camObject = new SceneObject();
	ActiveCamera = camObject->AddComponent<Camera>(90, 0.1f, 100.0f);

	camObject->GetTransform()->Translate(glm::vec3(0.0, 0.5f, 1.0f));

	auto lightObject = MakeLightObject(OUT &lightComponent);

	std::shared_ptr<SceneObject> floor(new SceneObject(glm::vec3(0, -1, -1), glm::quat()));
	std::vector<std::shared_ptr<Mesh>> floorMeshes;
	floorMeshes.emplace_back(std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures)));
	std::shared_ptr<Renderer> floorRenderer = floor->AddComponent<Renderer>(shader, floorMeshes);
	floorRenderer->ShadowMapCullingMode = GL_BACK;
	Renderers.push_back(floorRenderer);

	std::shared_ptr<SceneObject> cube(new SceneObject(glm::vec3(0, -1, -2.25), glm::quat()));
	std::shared_ptr<Renderer> cubeRenderer = cube->AddComponent<Renderer>(shader);
	cubeRenderer->ImportMeshesFromOBJ("Cube.obj");
	Renderers.push_back(cubeRenderer);

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

		gler::ProcessGLErrors(CLOGINFO);
		floorRenderer->Draw(*ActiveCamera);
		cubeRenderer->Draw(*ActiveCamera);
		gler::ProcessGLErrors(CLOGINFO);

		//RenderTextureToScreen(ShadowMapArray, GL_TEXTURE_2D_ARRAY);
		//RenderTextureToScreen(DEBUGSHADOW, GL_TEXTURE_2D);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader->Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
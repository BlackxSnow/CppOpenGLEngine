#include "Engine.h"

#include<iostream>
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
#include "EngineData.h"
#include "ETime.h"
#include "InputManager.h"
#include <filesystem>
#include "TextRendering.h"

std::map<std::string, std::shared_ptr<Shader>> Shaders;
GLFWwindow* Window;

Event<> OnGUIDraw;

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

void TryLoadInclude(const std::filesystem::directory_entry& entry, std::ifstream& ldFile)
{
	if (entry.path().extension() != ".glsl")
	{
		clog::Warning(CLOGINFO, "Attempted to load non GLSL file as include.");
		return;
	}
	ldFile = std::ifstream(entry.path().string().erase(0, 2), std::ios::ate | std::ios::in | std::ios::binary);

	if (!ldFile.is_open())
	{
		clog::Warning(CLOGINFO, "Include " + entry.path().filename().string() + " was unable to be loaded.");
		return;
	}

	std::string name = entry.path().filename().string();
	name.insert(0, "/");

	ldFile.seekg(0, std::ios::end);
	std::streampos size = ldFile.tellg();
	std::streampos dataLength = size;
	dataLength += 1;

	char* data = new char[dataLength];
	data[size] = '\0';
	ldFile.seekg(0, std::ios::beg);
	ldFile.read(data, size);
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, name.length(), name.c_str(), dataLength, data);
	delete[] data;
}

void LoadIncludes()
{
	const std::string path = "./Resources/Shaders/builtin/Includes/";
	const std::string userIncludePath = "./Resources/Shaders/Includes";
	std::ifstream ldFile;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		TryLoadInclude(entry, ldFile);
	}

	for (const auto& entry : std::filesystem::directory_iterator(userIncludePath))
	{
		TryLoadInclude(entry, ldFile);
	}
}

const int initialLightBufferSize = 16;
int lastLightBufferSize = -1;

GLuint GenerateLightBuffer()
{
	lastLightBufferSize = std::max(lastLightBufferSize * 2, initialLightBufferSize);

	while (lastLightBufferSize < SceneLights.size())
	{
		lastLightBufferSize *= 2;
	}

	GLuint lightSSBO;
	glGenBuffers(1, &lightSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(LightData) * lastLightBufferSize, NULL, GL_DYNAMIC_DRAW);
	return lightSSBO;
}

std::vector<LightData> lightData;

GLuint ShadowMapFrameBuffer;
int LastShadowMapCount = 0;
const int LightMapResolution = 1024;

void GenerateShadowMapArray(int mapCount)
{
	// Generate new texture array with correct depth.
	if (ShadowMapArray != -1)
	{
		glDeleteTextures(1, &ShadowMapArray);
	}
	glGenTextures(1, &ShadowMapArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ShadowMapArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16, LightMapResolution, LightMapResolution, mapCount, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColour[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColour);
	LastShadowMapCount = mapCount;
}

// There are many possible improvements to this shadow algorithm, some are:
//		- Cascaded shadow maps - Improves perspective aliasing, though is relatively complex
//		- Automatic calculation of tight fitting light frustrums
//		- Slope-scale depth bias (particularly accounting for near-far size
//		- Texel incremented light movement for directional lights
// Sources: 
//		https://docs.microsoft.com/en-us/windows/win32/dxtecharts/common-techniques-to-improve-shadow-depth-maps?redirectedfrom=MSDN
//		https://docs.microsoft.com/en-us/windows/win32/dxtecharts/cascaded-shadow-maps

void RenderLightShadowMaps(int mapCount)
{
	
	if (LastShadowMapCount != mapCount)
	{
		GenerateShadowMapArray(mapCount);
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

	int currentIndex = 0;
	for (Light* light : SceneLights)
	{
		if (light->isShadowCaster == false)
		{
			continue;
		}
		// Assign the current layer of the ShadowMapArray as the depth render target.
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ShadowMapArray, 0, currentIndex);
		
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightView;
		glm::mat4 lightProj;
		light->BuildLightMatrices(&lightView, &lightProj);
		glm::mat4 lightMatrix = lightProj * lightView;

		glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightMatrix));

		for (Renderer* renderer : Renderers)
		{
			glCullFace(renderer->ShadowMapCullingMode);
			renderer->Draw(*mapShader, true);
		}
		currentIndex++;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		clog::Error(CLOGINFO, "Framebuffer was not complete", false);
	}

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
	for (Light* light : SceneLights)
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

	auto& screenRender = Shaders["screenRender"];

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

void LoadBaseShaders()
{
	const std::string shaderPath = "Resources/Shaders/builtin/";
	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader(shaderPath + "default.vert", shaderPath + "default.frag"));
	std::shared_ptr<Shader> shadowMapShader = std::shared_ptr<Shader>(new Shader(shaderPath + "ShadowMap.vert", shaderPath + "ShadowMap.frag"));
	std::shared_ptr<Shader> screenRender = std::shared_ptr<Shader>(new Shader(shaderPath + "ScreenRender.vert", shaderPath + "ScreenRender.frag"));
	std::shared_ptr<Shader> text = std::shared_ptr<Shader>(new Shader(shaderPath + "Text.vert", shaderPath + "Text.frag"));

	Shaders.insert({ "default", shader });
	Shaders.insert({ "shadowMap", shadowMapShader });
	Shaders.insert({ "screenRender", screenRender });
	Shaders.insert({ "text", text });
}

int StartEngineLoop()
{
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(Window))
	{
		DeltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		//clog::Info(CLOGINFO, std::to_string(1 / DeltaTime));

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (IUpdatable* up : Updatable)
		{
			up->Update();
		}

		Physics::UpdatePhysicsSimulation();

		for (ILateUpdatable* lup : LateUpdatable)
		{
			lup->LateUpdate();
		}

		if (SceneLights.size() > lastLightBufferSize)
		{
			GenerateLightBuffer();
		}

		SetLightBufferData();

		gler::ProcessGLErrors(CLOGINFO);

		if (ActiveCamera != nullptr)
		{
			for (Renderer* renderer : Renderers)
			{
				if (renderer->IsEnabled())
				{
					renderer->Draw(*ActiveCamera);
				}
			}
		}
		gler::ProcessGLErrors(CLOGINFO);

		OnGUIDraw.Invoke();

		gler::ProcessGLErrors(CLOGINFO);

		glfwSwapBuffers(Window);
		ResetCursorDelta();
		glfwPollEvents();
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
	return 0;
}

void SetActiveCamera(std::shared_ptr<Camera> cam)
{
	ActiveCamera = cam.get();
}

int InitialiseEngine()
{
	InitWindow(&Window);

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		glfwDestroyWindow(Window);
		glfwTerminate();
		return -1;
	}
	LoadIncludes();
	InitialiseInput();
	LoadBaseShaders();
	txt::Init();

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenFramebuffers(1, &ShadowMapFrameBuffer);
	ShadowCasterMatrices = std::shared_ptr<std::vector<glm::mat4>>(new std::vector<glm::mat4>);

	GenerateLightBuffer();

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(Window);
}
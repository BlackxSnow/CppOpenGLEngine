#include "Main.h"

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Defs.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
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

std::string vec4ToString(glm::vec4* vector)
{
	std::string result = std::string();
	result.append(std::to_string(vector->x));
	result.append(", ");
	result.append(std::to_string(vector->y));
	result.append(", ");
	result.append(std::to_string(vector->z));
	result.append(", ");
	result.append(std::to_string(vector->w));
	return result;
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

	Shader shader("default.vert", "default.frag");
	VertexArray vertArray;
	vertArray.Bind();

	VertexBuffer vertBuffer(vertices, sizeof(vertices));
	ElementBuffer elementBuffer(indices, sizeof(indices));

	vertArray.LinkAttrib(vertBuffer, 0, 3, GL_FLOAT, sizeof(GLfloat) * 11, 0);
	vertArray.LinkAttrib(vertBuffer, 1, 3, GL_FLOAT, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 3));
	vertArray.LinkAttrib(vertBuffer, 2, 2, GL_FLOAT, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 6));
	vertArray.LinkAttrib(vertBuffer, 3, 3, GL_FLOAT, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 8));
	vertArray.Unbind();
	vertBuffer.Unbind();
	elementBuffer.Unbind();

	Texture* texture = new Texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	shader.Activate();
	GLuint tex0Location = glGetUniformLocation(shader.ID, "tex0");
	glUniform1i(tex0Location, 0);

	SceneObject* camObject = new SceneObject();
	ActiveCamera = camObject->AddComponent<Camera>(90, 0.1f, 100.0f);

	camObject->GetTransform()->Translate(glm::vec3(0.0, 0.5f, 1.0f));

	std::shared_ptr<SpotLight> lightComponent;
	auto lightObject = MakeLightObject(OUT &lightComponent);

	std::vector<LightData> lights;

	GLuint lightSSBO;
	glGenBuffers(1, &lightSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(LightData) * 10, NULL, GL_DYNAMIC_DRAW);

	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0, 0, -1));

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);

	float deltaTime = 0;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Activate();

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

		lights.clear();
		lights.push_back(lightComponent->BuildLightData());
		int lightCount = lights.size();
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &lightCount);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(LightData) * lights.size(), &lights[0]);

		//model = glm::rotate(model, glm::radians(90.0f * deltaTime), glm::vec3(0, 1, 0));
		glm::mat4 view;
		glm::mat4 proj;
		ActiveCamera->BuildMatrices(&view, &proj);
		glm::mat4 renderMatrix = proj * view * model;

		glm::vec3 camPos = ActiveCamera->GetSceneObject()->GetTransform()->GetWorldPosition();
		int camPosLocation = glGetUniformLocation(shader.ID, "camPosition");
		glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

		int modelMatrixLocation = glGetUniformLocation(shader.ID, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

		int renderMatrixLocation = glGetUniformLocation(shader.ID, "renderMatrix");
		glUniformMatrix4fv(renderMatrixLocation, 1, GL_FALSE, glm::value_ptr(renderMatrix));

		vertArray.Bind();
		glBindTexture(GL_TEXTURE_2D, texture->ID);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vertArray.Delete();
	vertBuffer.Delete();
	elementBuffer.Delete();
	shader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#include "InputManager.h"

#include <unordered_map>
#include <tuple>

#include "Event.h"
#include "ConsoleLogging.h"

struct InputKey
{
	int keyID;
	int actionID;

	InputKey(int key, int action)
	{
		keyID = key;
		actionID = action;
	}
};

bool operator==(const InputKey& a, const InputKey& b)
{
	return a.keyID == b.keyID && a.actionID == b.actionID;
}

namespace std
{
	template<>
	struct hash<InputKey>
	{
		std::size_t operator()(const InputKey& k) const
		{
			std::string hashString = k.keyID + ";" + k.actionID;
			return std::hash<string>()(hashString);
		}
	};
}

std::unordered_map<InputKey, std::shared_ptr<Event<int>>> KeyEvents;
glm::vec2 LastCursorPos;
glm::vec2 CursorDelta;

void GLFWKeyCallback(GLFWwindow* window, int glfwKey, int scanCode, int action, int mods)
{
	InputKey key(glfwKey, action);
	try
	{
		std::shared_ptr<Event<int>> keyEvent = KeyEvents.at(key);

		keyEvent->Invoke(mods);
	}
	catch (const std::exception&)
	{

	}
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputKey key(button, action);
	try
	{
		std::shared_ptr<Event<int>> keyEvent = KeyEvents.at(key);

		keyEvent->Invoke(mods);
	}
	catch (const std::exception&)
	{

	}
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	CursorDelta = glm::vec2(xpos, ypos) - LastCursorPos;
	LastCursorPos = glm::vec2(xpos, ypos);
}

glm::vec2 GetPointerDelta()
{
	return CursorDelta;
}

void InitialiseInput()
{
	glfwSetCursorPosCallback(Window, CursorPositionCallback);
	glfwSetKeyCallback(Window, GLFWKeyCallback);
	glfwSetMouseButtonCallback(Window, GLFWMouseButtonCallback);
}

void RegisterInputCallback(int glfwKey, int action, std::string name, std::function<void(int)> callback)
{
	InputKey key(glfwKey, action);

	std::shared_ptr<Event<int>> keyEvent;
	
	try
	{
		keyEvent = KeyEvents.at(key);
	}
	catch (const std::out_of_range&)
	{
		keyEvent = KeyEvents.insert({ key, std::shared_ptr<Event<int>>(new Event<int>()) }).first->second;
	}

	keyEvent->Register(name, callback);
}

bool DeregisterInputCallback(int glfwKey, int action, std::string name)
{
	InputKey key(glfwKey, action);

	try
	{
		std::shared_ptr<Event<int>> keyEvent = KeyEvents.at(key);

		return keyEvent->Deregister(name);
	}
	catch (const std::out_of_range&)
	{
		return false;
	}
}

void ResetCursorDelta()
{
	CursorDelta = glm::vec2(0, 0);
}



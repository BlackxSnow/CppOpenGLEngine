#pragma once

#include <functional>

#include "Engine.h"

glm::vec2 GetPointerDelta();

void InitialiseInput();

/// <summary>
/// Register a callback for a key or mouse button input.
/// </summary>
/// <param name="glfwKey">- GLFW ID for key or mouse button.</param>
/// <param name="action">- GLFW ID for input actuation type.</param>
/// <param name="callback">- Callback function.</param>
void RegisterInputCallback(int glfwKey, int action, std::string name, std::function<void(int)> callback);

/// <summary>
/// Dregister an existing input callback. Returns whether successful.
/// </summary>
/// <param name="glfwKey">- GLFW ID for key or mouse button.</param>
/// <param name="action">- GLFW ID for input actuation type.</param>
/// <param name="callback">- Callback function.</param>
bool DeregisterInputCallback(int glfwKey, int action, std::string name);

void ResetCursorDelta();
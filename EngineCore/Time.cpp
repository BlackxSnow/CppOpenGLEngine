#include "Time.h"
#include "GLFW/glfw3.h"

float applicationStartTime;
bool isInitialised = false;
void Init()
{
    if (!isInitialised)
    {
        applicationStartTime = glfwGetTime();
    }
}

float DeltaTime = 0;

float Time()
{
    return glfwGetTime() - applicationStartTime;
}

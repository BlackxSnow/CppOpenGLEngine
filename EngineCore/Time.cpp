#include "Time.h"
#include "GLFW/glfw3.h"

float applicationStartTime;
bool isTimeInitialised = false;
void InitTime()
{
    if (!isTimeInitialised)
    {
        applicationStartTime = glfwGetTime();
    }
}

float DeltaTime = 0;

float Time()
{
    return glfwGetTime() - applicationStartTime;
}

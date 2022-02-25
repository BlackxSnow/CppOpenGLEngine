#include "EMath.h"

float SquareDistance(glm::vec3 a, glm::vec3 b)
{
    glm::vec3 between = b - a;

    return pow(between.x, 2) + pow(between.y, 2) + pow(between.z, 2);
}

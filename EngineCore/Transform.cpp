#include "Transform.h"
#include <glm/gtx/quaternion.hpp>

COMPONENT_DEFINITION(Component, Transform)


glm::vec3 Transform::GetPosition()
{
    return Position;
}

void Transform::SetPosition(glm::vec3 position)
{
    Position = position;
}

glm::quat Transform::GetRotation()
{
    return Rotation;
}

void Transform::SetRotation(glm::quat quaternion)
{
    Rotation = quaternion;
}

glm::vec3 Transform::GetScale()
{
    return Scale;
}

void Transform::SetScale(glm::vec3 scale)
{
    Scale = scale;
}

glm::mat4 Transform::BuildModelMatrix()
{
    glm::mat4 result = glm::mat4(1);

    result = glm::scale(glm::translate(result, GetWorldPosition()) * glm::toMat4(GetWorldRotation()), Scale);
    return result;
}

glm::vec3 Transform::GetWorldPosition()
{
    glm::vec3 current = Position;
    Transform* nextParent = GetParent();

    while (nextParent != nullptr)
    {
        current += nextParent->GetPosition();
        nextParent = nextParent->GetParent();
    }

    return current;
}

glm::quat Transform::GetWorldRotation()
{
    glm::quat result = glm::identity<glm::quat>();

    std::vector<glm::quat> hierarchy;
    hierarchy.push_back(Rotation);

    Transform* parent = GetParent();

    while (parent != nullptr)
    {
        hierarchy.push_back(parent->GetRotation());
        parent = parent->GetParent();
    }

    // Multiply quaternions from highest parent to this child.
    for (int i = hierarchy.size() - 1; i >= 0; i--)
    {
        result *= hierarchy[i];
    }

    return result;
}

glm::vec3 Transform::Forward()
{
    glm::vec3 forward = glm::vec3(0,0,-1);
    forward = GetWorldRotation() * forward;

    return forward;
    
}

glm::vec3 Transform::Up()
{
    glm::vec3 up = glm::vec3(0, 1, 0);
    up = GetWorldRotation() * up;
    return up;
}

glm::vec3 Transform::Right()
{
    glm::vec3 right = glm::vec3(1, 0, 0);
    right = GetWorldRotation() * right;
    return right;
}

void Transform::Translate(glm::vec3 translation)
{
    Position += translation;
}

void Transform::Rotate(glm::vec3 euler)
{
    Rotation *= glm::quat(euler);
}

Transform* Transform::GetParent()
{
    return Parent;
}

Transform::Transform(SceneObject* attachedTo) : Component(std::move(attachedTo))
{
    Position = glm::vec3(0, 0, 0);
    Rotation = glm::identity<glm::mat4x4>();
    Scale = glm::vec3(1, 1, 1);
    Parent = nullptr;
}

Transform::Transform(SceneObject* attachedTo, glm::vec3 pos, glm::quat rot, Transform* parent) : Component(std::move(attachedTo))
{
    Position = pos;
    if (rot.x == 0 && rot.y == 0 && rot.z == 0 && rot.w == 0)
    {
        rot = glm::quat(1, 0, 0, 0);
    }
    Rotation = rot;
    Scale = glm::vec3(1, 1, 1);
    Parent = parent;
}

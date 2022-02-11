#include "Transform.h"

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

glm::vec3 Transform::GetForwardVector()
{
    glm::vec3 forward = glm::vec3(0,0,-1);
    forward = GetWorldRotation() * forward;
    Transform* nextParent = GetParent();

    return forward;
    
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
    Rotation = rot;
    Scale = glm::vec3(1, 1, 1);
    Parent = parent;
}

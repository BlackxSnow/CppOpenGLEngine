#include "Component.h"

const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

SceneObject* Component::GetSceneObject()
{
    return AttachedObject;
}

Component::Component(SceneObject* attached)
{
    AttachedObject = attached;
    Enable();
}

bool Component::IsClassType(const std::size_t classType) const
{
    return classType == Component::Type;
    return true;
}

void Component::Disable()
{
    IsActive = false;
}

void Component::Enable()
{
    IsActive = true;
}

bool Component::IsEnabled()
{
    return IsActive;
}

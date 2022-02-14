#include "SceneObject.h"

Transform* SceneObject::GetTransform()
{
    return ObjectTransform;
}

SceneObject::SceneObject()
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this);
}

SceneObject::SceneObject(glm::vec3 pos, glm::quat rot, Transform* parent)
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this, pos, rot, parent);
}

SceneObject::SceneObject(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, Transform* parent)
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this, pos, glm::quatLookAt(forward, up), parent);
}

SceneObject::~SceneObject()
{
    delete Components;
}
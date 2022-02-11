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

SceneObject::~SceneObject()
{
    delete Components;
}
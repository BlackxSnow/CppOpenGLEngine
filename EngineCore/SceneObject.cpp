#include "SceneObject.h"
#include "EngineData.h"

Transform* SceneObject::GetTransform()
{
    return ObjectTransform;
}

void Register(SceneObject* obj)
{
    SceneObjects.push_back(obj);
}

void Deregister(SceneObject* obj)
{
    SceneObjects.remove(obj);
}

SceneObject::SceneObject()
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this);
    Register(this);
}

SceneObject::SceneObject(glm::vec3 pos, glm::quat rot, Transform* parent)
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this, pos, rot, parent);
    Register(this);
}

SceneObject::SceneObject(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, Transform* parent)
{
    Components = new std::vector<std::shared_ptr<Component>>();
    ObjectTransform = new Transform(this, pos, glm::quatLookAt(forward, up), parent);
    Register(this);
}

SceneObject::~SceneObject()
{
    Deregister(this);
    delete Components;
}
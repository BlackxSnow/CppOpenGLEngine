#pragma once

#include<vector>
#include<memory>

#include "Component.h"
#include "Transform.h"

class SceneObject
{
public:
	Transform* GetTransform();
	
	std::vector<std::shared_ptr<Component>>* Components;

	/// <summary>
	/// Add a component of provided template type and pass 'params' to its constructor.
	/// </summary>
	/// <typeparam name="ComponentType"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...params"></param>
	/// <returns></returns>
	template<class ComponentType, typename... Args>
	std::shared_ptr<ComponentType> AddComponent(Args&&... params);

	/// <summary>
	/// Get a component of provided template type if it exists, otherwise return nullptr.
	/// </summary>
	/// <typeparam name="ComponentType"></typeparam>
	/// <returns></returns>
	template<class ComponentType>
	ComponentType* GetComponent();

	/// <summary>
	/// Attempt to remove a component of provided template type and return whether the operation was successful.
	/// </summary>
	/// <typeparam name="ComponentType"></typeparam>
	/// <returns></returns>
	template<class ComponentType>
	bool RemoveComponent();

	SceneObject();
	SceneObject(glm::vec3 pos, glm::quat rot, Transform* parent = nullptr);

	~SceneObject();

private:
	Transform* ObjectTransform;
};


template<class ComponentType, typename... Args>
std::shared_ptr<ComponentType> SceneObject::AddComponent(Args&&... params)
{
	// Create the component and a unique_ptr to it using params.
	// std::forward ensures params maintains its l/rvalue status.
	std::shared_ptr<ComponentType> component = std::make_shared<ComponentType>(this, std::forward<Args>(params)...);
	Components->emplace_back(component);
	return component;
}

template<class ComponentType>
ComponentType* SceneObject::GetComponent()
{
	for (Component&& component : Components)
	{
		if (component->IsClassType(ComponentType::Type))
		{
			return *static_cast<ComponentType*>(component.get());
		}
	}
	return *std::unique_ptr<ComponentType>(nullptr);
}

template<class ComponentType>
bool SceneObject::RemoveComponent()
{
	if (Components.empty())
	{
		return false;
	}

	auto& index = std::find_if(Components.begin(), Components.end(),
		[classType = ComponentType::Type](Component& component)
	{
		return component->IsClassType(classType);
	});

	bool success = index != Components.end();

	if (success)
	{
		Components.erase(index);
	}

	return success;
}
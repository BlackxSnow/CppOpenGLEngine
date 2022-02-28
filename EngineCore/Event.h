#pragma once

#include<functional>

/// <summary>
/// Registerable event handler object. Searchable by name.
/// </summary>
/// <typeparam name="...Types"></typeparam>
template<typename... Types>
struct EventHandle
{
	std::string Name;
	std::function<void(Types...)> Function;

	EventHandle(std::string name, std::function<void(Types...)> func)
	{
		Name = name;
		Function = func;
	}
};

/// <summary>
/// Invokable and registerable generic event class.
/// </summary>
/// <typeparam name="...Types"></typeparam>
template<typename... Types>
class Event
{
public:
	/// <summary>
	/// Register a new callback to the event under 'name'.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="registree"></param>
	void Register(std::string name, std::function<void(Types...)> registree)
	{
		Registrees.emplace_back(name, registree);
	}
	/// <summary>
	/// Remove an existing registree from the event.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool Deregister(std::string name)
	{
		//auto pos = std::find(Registrees.begin(), Registrees.end(), registree);
		auto pos = std::find_if(Registrees.begin(), Registrees.end(), [name](EventHandle<Types...> r) { return r.Name == name; });
		if (pos != Registrees.end())
		{
			Registrees.erase(pos);
			return true;
		}
		return false;
	}
	/// <summary>
	/// Call all registrees of this event with the provided parameters.
	/// </summary>
	/// <param name="...types"></param>
	void Invoke(Types... types)
	{
		for (auto& handle : Registrees)
		{
			handle.Function(types...);
		}
	}
private:
	std::vector<EventHandle<Types...>> Registrees = std::vector<EventHandle<Types...>>();

};
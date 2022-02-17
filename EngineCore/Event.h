#pragma once

#include<functional>

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

template<typename... Types>
class Event
{
public:
	void Register(std::string name, std::function<void(Types...)> registree)
	{
		Registrees.emplace_back(name, registree);
	}
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
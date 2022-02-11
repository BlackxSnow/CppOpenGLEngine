#pragma once

#include<functional>

template<typename... Types>
class Event
{
public:
	void Register(std::function<void(Types...)> registree)
	{
		Registrees.push_back(registree);
	}
	void Invoke(Types... types)
	{
		for (auto& func : Registrees)
		{
			func(types...);
		}
	}
private:
	std::vector<std::function<void(Types...)>> Registrees;

};
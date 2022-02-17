#include "Callbacks.h"
#include "EngineData.h"

IUpdatable::IUpdatable()
{
	Updatable.push_back(this);
}

IUpdatable::~IUpdatable()
{
	Updatable.remove(this);
}

ILateUpdatable::ILateUpdatable()
{
	LateUpdatable.push_back(this);
}

ILateUpdatable::~ILateUpdatable()
{
	LateUpdatable.remove(this);
}

IFixedUpdatable::IFixedUpdatable()
{
	FixedUpdatable.push_back(this);
}

IFixedUpdatable::~IFixedUpdatable()
{
	FixedUpdatable.remove(this);
}

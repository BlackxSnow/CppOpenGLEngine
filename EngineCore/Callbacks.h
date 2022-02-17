#pragma once

/// <summary>
/// Registers class for frame updates.
/// </summary>
class IUpdatable
{
public:
	virtual void Update() = 0;

	IUpdatable();
	~IUpdatable();
};

/// <summary>
/// Registers class for callbacks after physics calculations.
/// </summary>
class ILateUpdatable
{
public:
	virtual void LateUpdate() = 0;

	ILateUpdatable();
	~ILateUpdatable();
};

class IFixedUpdatable
{
public:
	virtual void FixedUpdate() = 0;

	IFixedUpdatable();
	~IFixedUpdatable();
};
#pragma once

/// <summary>
/// Inherit from this to register class for pre-physics frame updates.
/// </summary>
class IUpdatable
{
public:
	virtual void Update() = 0;

	IUpdatable();
	~IUpdatable();
};

/// <summary>
/// Inherit from this to register class for callbacks after physics calculations.
/// </summary>
class ILateUpdatable
{
public:
	virtual void LateUpdate() = 0;

	ILateUpdatable();
	~ILateUpdatable();
};

/// <summary>
/// Inheirt from this to register class for callbacks on physics update.
/// </summary>
class IFixedUpdatable
{
public:
	virtual void FixedUpdate() = 0;

	IFixedUpdatable();
	~IFixedUpdatable();
};
#pragma once

#include "Component.h"

class Renderer3D : public Component
{
	COMPONENT_DECLARATION(Renderer3D)

public:
	Mesh* MeshData;
};
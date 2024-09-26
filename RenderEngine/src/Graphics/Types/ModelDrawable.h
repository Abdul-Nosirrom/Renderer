#pragma once
#include "Core.h"
#include "PrimitiveGeometry.h"


class RENDERENGINE_API ModelDrawable : public PrimitiveDrawable
{
public:
	ModelDrawable(Graphics& gfx, const std::string& modelPath);
};
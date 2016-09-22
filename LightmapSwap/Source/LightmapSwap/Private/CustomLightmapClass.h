#pragma once

#include "LightMap.h"

class FCustomLightmapClass : public FLightMap2D
{
public:
	void setTexture(int textureIndex, UTexture2D *newTexture);

	void copyDataFromOtherLightmap(FLightMap2D *otherLightmap);
};
#include "LightmapSwapModule.h"
#include "CustomLightmapClass.h"


void FCustomLightmapClass::setTexture(int textureIndex, UTexture2D *newTexture)
{
	Textures[textureIndex] = (ULightMapTexture2D *) newTexture;
}

void FCustomLightmapClass::copyDataFromOtherLightmap(FLightMap2D *otherLightmap)
{
	FLightMapInteraction interaction = otherLightmap->GetInteraction(ERHIFeatureLevel::SM5);

	LightGuids = otherLightmap->LightGuids;
	bAllowHighQualityLightMaps = otherLightmap->AllowsHighQualityLightmaps();
	ScaleVectors[0] = interaction.GetScaleArray()[0];
	ScaleVectors[1] = interaction.GetScaleArray()[1];
	ScaleVectors[2] = interaction.GetScaleArray()[2];
	ScaleVectors[3] = interaction.GetScaleArray()[3];

	AddVectors[0] = interaction.GetAddArray()[0];
	AddVectors[1] = interaction.GetAddArray()[1];
	AddVectors[2] = interaction.GetAddArray()[2];
	AddVectors[3] = interaction.GetAddArray()[3];

	CoordinateScale[0] = interaction.GetCoordinateScale()[0];
	CoordinateScale[1] = interaction.GetCoordinateScale()[1];

	CoordinateBias[0] = interaction.GetCoordinateBias()[0];
	CoordinateBias[1] = interaction.GetCoordinateBias()[1];
}


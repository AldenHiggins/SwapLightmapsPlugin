#pragma once

#include "GameFramework/Actor.h"
#include "SceneTypes.h"
#include "LightMap.h"
#include "CustomLightmapClass.h"
#include "Archive.h"
#include "LightmapSwapActor.generated.h"

UCLASS()
class  ALightmapSwapActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CaptureSettings)
	TArray<AActor *> onlyRenderTheseActors;

	UFUNCTION(BlueprintCallable, Category = "Lightmap Saving")
	TArray<UTexture2D *> getLightMapTextureArray();

	UFUNCTION(BlueprintCallable, Category = "Lightmap Loading")
	void loadLightMap();

	UPROPERTY()
	UTexture2D *firstCopiedTexture;

	UPROPERTY()
	UTexture2D *secondCopiedTexture;

	FBufferArchive firstLightmapBinaryData;

	FBufferArchive secondLightmapBinaryData;

private:

	FCustomLightmapClass myCoolLightmap;

	UPROPERTY()
	TArray<AActor *> actorsInScene;

	UPROPERTY()
	UPointLightComponent* PointLightComponent;

	UPROPERTY()
	UStaticMeshComponent *staticMeshToApplyLightmapsTo;

	UPROPERTY()
	UStaticMeshComponent *staticMeshToGetLightmapFrom;

	void copyTextureData(UTexture2D *sourceTexture, UTexture2D *targetTexture);

};

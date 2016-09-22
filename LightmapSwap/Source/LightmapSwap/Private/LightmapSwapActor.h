#pragma once

#include "GameFramework/Actor.h"
#include "SceneTypes.h"
#include "LightMap.h"
#include "CustomLightmapClass.h"
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

private:

	FCustomLightmapClass myCoolLightmap;

	UPROPERTY()
	UTexture2D *firstCopiedTexture;

	UPROPERTY()
	UTexture2D *secondCopiedTexture;

	UPROPERTY()
	TArray<AActor *> actorsInScene;

	UPROPERTY()
	UStaticMeshComponent *staticMeshToApplyLightmapsTo;

	UPROPERTY()
	UStaticMeshComponent *staticMeshToGetLightmapFrom;

};

#pragma once

#include "GameFramework/Actor.h"
#include "SceneTypes.h"
#include "LightMap.h"
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


	UStaticMeshComponent *staticMeshToApplyLightmapsTo;
	TArray<FLightMapRef> lightMaps;
};
#include "LightmapSwapModule.h"
#include "LightmapSwapActor.h"

ALightmapSwapActor::ALightmapSwapActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

TArray<UTexture2D *> ALightmapSwapActor::getLightMapTextureArray()
{
	for (TActorIterator<AActor> ActorItr(GWorld); ActorItr; ++ActorItr)
	{
		AActor *theActor = *ActorItr;

		UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *theActor->GetName());

		if (theActor->GetName() != TEXT("Floor_14"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Not the floor so who cares"));
			continue;
		}

		TArray<UActorComponent *> staticMeshes = theActor->GetComponentsByClass(UStaticMeshComponent::StaticClass());

		if (staticMeshes.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No static mesh found"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Yes static meshes found: %d"), staticMeshes.Num());
		}

		for (int32 ComponentIndex = 0; ComponentIndex < staticMeshes.Num(); ComponentIndex++)
		{
			UStaticMeshComponent* staticMeshComponent = (UStaticMeshComponent *)staticMeshes[ComponentIndex];

			if (!staticMeshComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("Static mesh component isn't valid something bad happened"));
				continue;
			}


			UE_LOG(LogTemp, Warning, TEXT("LODs found: %d"), staticMeshComponent->LODData.Num());

			lightMaps.Empty();

			staticMeshToApplyLightmapsTo = staticMeshComponent;

			for (int32 lodIndex = 0; lodIndex < staticMeshComponent->LODData.Num(); lodIndex++)
			{
				lightMaps.Add(staticMeshComponent->LODData[lodIndex].LightMap);
			}

		}
	}

	TArray<UTexture2D*> LightMapsAndShadowMaps;
	GWorld->GetLightMapsAndShadowMaps(GWorld->GetCurrentLevel(), LightMapsAndShadowMaps);

	return LightMapsAndShadowMaps;
}

void ALightmapSwapActor::loadLightMap()
{
	for (int32 lodIndex = 0; lodIndex < staticMeshToApplyLightmapsTo->LODData.Num(); lodIndex++)
	{
		if (lodIndex >= lightMaps.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("More LOD levels than saved lightmaps, something went wrong yo!"));
		}

		staticMeshToApplyLightmapsTo->LODData[lodIndex].LightMap = lightMaps[lodIndex];
	}
}


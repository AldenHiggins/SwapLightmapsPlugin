#include "LightmapSwapModule.h"
#include "AssetRegistryModule.h"
#include "ImageUtils.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "LightmapSwapActor.h"


ALightmapSwapActor::ALightmapSwapActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

TArray<UTexture2D *> ALightmapSwapActor::getLightMapTextureArray()
{
	// Clear out the actors in the scene
	actorsInScene.Empty();

	for (TActorIterator<AActor> ActorItr(GWorld); ActorItr; ++ActorItr)
	{
		AActor *theActor = *ActorItr;

		// Add this actor to our list so we can track info about it...nsa style
		actorsInScene.Add(theActor);

		UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *theActor->GetName());

		if (theActor->GetName() == TEXT("Chair"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found the chair!!"));
			staticMeshToGetLightmapFrom = (UStaticMeshComponent *) theActor->GetComponentByClass(UStaticMeshComponent::StaticClass());;
			continue;
		}

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

			staticMeshToApplyLightmapsTo = staticMeshComponent;

			// Initialize the package name
			FString Name;
			FString PackageName;
			FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
			AssetToolsModule.Get().CreateUniqueAssetName(TEXT("/Game/SavedLightmaps/MyCoolNewTexture"), TEXT("MyCoolNewTexture"), PackageName, Name);

			// Create the new texture
			EObjectFlags Flags = RF_Public | RF_Standalone;
			UPackage* Pkg = CreatePackage(nullptr, *PackageName);

			UTexture2D *oldLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(0);
			firstCopiedTexture = DuplicateObject(oldLightmapTexture, Pkg, TEXT("MyCoolNewTexture"));

			firstCopiedTexture->MarkPackageDirty();

			FAssetRegistryModule::AssetCreated(firstCopiedTexture);

			FString Name2;
			FString PackageName2;
			AssetToolsModule.Get().CreateUniqueAssetName(TEXT("/Game/SavedLightmaps/MyCoolNewTexture2"), TEXT("MyCoolNewTextureTheSecond"), PackageName2, Name2);
			UPackage* SecondPackage = CreatePackage(nullptr, *PackageName2);

			UTexture2D *secondLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(1);
			secondCopiedTexture = DuplicateObject(secondLightmapTexture, SecondPackage, TEXT("MyCoolNewTextureTheSecond"));

			secondCopiedTexture->MarkPackageDirty();

			FAssetRegistryModule::AssetCreated(secondCopiedTexture);

			myCoolLightmap.setTexture(0, firstCopiedTexture);
			myCoolLightmap.setTexture(1, secondCopiedTexture);

			myCoolLightmap.copyDataFromOtherLightmap(staticMeshComponent->LODData[0].LightMap->GetLightMap2D());
		}
	}

	for (TActorIterator<AActor> ActorItr(GWorld); ActorItr; ++ActorItr)
	{
		AActor *theActor = *ActorItr;

		UPointLightComponent * lightComponent = (UPointLightComponent *) theActor->GetComponentByClass(UPointLightComponent::StaticClass());

		if (lightComponent != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("Light component found on actor: %s"), *theActor->GetName());
			PointLightComponent = lightComponent;
		}

	}

	TArray<UTexture2D*> LightMapsAndShadowMaps;
	GWorld->GetLightMapsAndShadowMaps(GWorld->GetCurrentLevel(), LightMapsAndShadowMaps);

	return LightMapsAndShadowMaps;
}

void ALightmapSwapActor::loadLightMap()
{
	if (staticMeshToApplyLightmapsTo == NULL || staticMeshToGetLightmapFrom == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to load lightmap but one hasn't been saved!"));
		return;
	}

	TArray<UTexture2D*> LightMapsAndShadowMaps;
	GWorld->GetLightMapsAndShadowMaps(GWorld->GetCurrentLevel(), LightMapsAndShadowMaps);

	
	uint8* TexData = firstCopiedTexture->Source.LockMip(0);

	for (int mipIndex = 1; mipIndex < firstCopiedTexture->GetNumMips(); mipIndex++)
	{
		firstCopiedTexture->Source.LockMip(mipIndex);
	}

	//LightMapsAndShadowMaps[0]->Source.LockMip(0);

	for (int mipIndex = 0; mipIndex < LightMapsAndShadowMaps[0]->GetNumMips(); mipIndex++)
	{
		LightMapsAndShadowMaps[0]->Source.LockMip(mipIndex);
	}

	//firstCopiedTexture->TemporarilyDisableStreaming();
	LightMapsAndShadowMaps[0]->TemporarilyDisableStreaming();

	FUpdateTextureRegion2D Region(0, 0, 0, 0, LightMapsAndShadowMaps[0]->GetSizeX(), LightMapsAndShadowMaps[0]->GetSizeY());
	LightMapsAndShadowMaps[0]->UpdateTextureRegions(0, 1, &Region, LightMapsAndShadowMaps[0]->GetSizeX() * 4, 16, TexData);
	//FlushRenderingCommands();

	for (int mipIndex = 1; mipIndex < LightMapsAndShadowMaps[0]->GetNumMips(); mipIndex++)
	{
		LightMapsAndShadowMaps[0]->Source.LockMip(mipIndex);
	}

	for (int mipIndex = 0; mipIndex < firstCopiedTexture->GetNumMips(); mipIndex++)
	{
		firstCopiedTexture->Source.UnlockMip(mipIndex);
	}


	for (int mipIndex = 0; mipIndex < LightMapsAndShadowMaps[0]->GetNumMips(); mipIndex++)
	{
		LightMapsAndShadowMaps[0]->Source.UnlockMip(mipIndex);
	}

	for (int actorIndex = 0; actorIndex < actorsInScene.Num(); actorIndex++)
	{
		actorsInScene[actorIndex]->ReregisterAllComponents();
	}

	//for (int32 lodIndex = 0; lodIndex < staticMeshToApplyLightmapsTo->LODData.Num(); lodIndex++)
	//{
	//	//TRefCountPtr<FLightMap2D> LightMap;
	//	FLightMap2D *lightmapPointer = &myCoolLightmap;
	//	TRefCountPtr<FLightMap2D> lightmapRefCountPointer = lightmapPointer;
	//	staticMeshToApplyLightmapsTo->LODData[lodIndex].LightMap = lightmapRefCountPointer;
	//	staticMeshToApplyLightmapsTo->MarkRenderStateDirty();
	//}
}


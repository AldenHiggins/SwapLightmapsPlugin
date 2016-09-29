#include "LightmapSwapModule.h"
#include "AssetRegistryModule.h"
#include "ImageUtils.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "Engine/Texture2D.h"
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

			UTexture2D *firstLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(0);

			FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
			FString PathName = TEXT("/Game");

			FString ObjectName;
			FString NewPackageName;
			AssetToolsModule.Get().CreateUniqueAssetName(PathName + "/" + TEXT("NewTexture"), TEXT(""), NewPackageName, ObjectName);

			// create one on skeleton folder
			UObject* NewAsset = AssetToolsModule.Get().DuplicateAsset(ObjectName, PathName, firstLightmapTexture);

			NewAsset->MarkPackageDirty();

			FAssetRegistryModule::AssetCreated(NewAsset);


			//UTexture2D *firstLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(0);

			//firstLightmapBinaryData << firstLightmapTexture;

			//UTexture2D *secondLightmapTexture = staticMeshComponent->LODData[1].LightMap->GetLightMap2D()->GetTexture(0);

			//secondLightmapBinaryData << secondLightmapTexture;


			//UE_LOG(LogTemp, Warning, TEXT("LODs found: %d"), staticMeshComponent->LODData.Num());

			//staticMeshToApplyLightmapsTo = staticMeshComponent;

			//// Initialize the package name
			//FString Name;
			//FString PackageName;
			//FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
			//AssetToolsModule.Get().CreateUniqueAssetName(TEXT("/Game/SavedLightmaps/MyCoolNewTexture"), TEXT("MyCoolNewTexture"), PackageName, Name);

			//// Create the new texture
			//EObjectFlags Flags = RF_Public | RF_Standalone;
			//UPackage* Pkg = CreatePackage(nullptr, *PackageName);

			//UTexture2D *oldLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(0);
			//firstCopiedTexture = DuplicateObject(oldLightmapTexture, Pkg, TEXT("MyCoolNewTexture"));

			//firstCopiedTexture->MarkPackageDirty();

			//FAssetRegistryModule::AssetCreated(firstCopiedTexture);

			//FString Name2;
			//FString PackageName2;
			//AssetToolsModule.Get().CreateUniqueAssetName(TEXT("/Game/SavedLightmaps/MyCoolNewTexture2"), TEXT("MyCoolNewTextureTheSecond"), PackageName2, Name2);
			//UPackage* SecondPackage = CreatePackage(nullptr, *PackageName2);

			//UTexture2D *secondLightmapTexture = staticMeshComponent->LODData[0].LightMap->GetLightMap2D()->GetTexture(1);
			//secondCopiedTexture = DuplicateObject(secondLightmapTexture, SecondPackage, TEXT("MyCoolNewTextureTheSecond"));

			//secondCopiedTexture->MarkPackageDirty();

			//FAssetRegistryModule::AssetCreated(secondCopiedTexture);

			//myCoolLightmap.setTexture(0, firstCopiedTexture);
			//myCoolLightmap.setTexture(1, secondCopiedTexture);

			//myCoolLightmap.copyDataFromOtherLightmap(staticMeshComponent->LODData[0].LightMap->GetLightMap2D());
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
	FStringAssetReference AssetPath(TEXT("/Game/TestMap1.TestMap1"));

	firstCopiedTexture = (UTexture2D *) AssetPath.TryLoad();

	UE_LOG(LogTemp, Warning, TEXT("Trying to load a texture yo"));

	TArray<UTexture2D*> LightMapsAndShadowMaps;
	GWorld->GetLightMapsAndShadowMaps(GWorld->GetCurrentLevel(), LightMapsAndShadowMaps);

	//UTexture2D* recoveredLightmap = UTexture2D::CreateTransient(512, 512, PF_B8G8R8A8);
	////UTexture2D recoveredLightmap;

	//FMemoryReader reading = FMemoryReader(firstLightmapBinaryData, true);
	//reading.Seek(0);

	//recoveredLightmap->Serialize(reading);



	copyTextureData(firstCopiedTexture, LightMapsAndShadowMaps[0]);

	//copyTextureData(secondCopiedTexture, LightMapsAndShadowMaps[1]);
}

void ALightmapSwapActor::copyTextureData(UTexture2D *sourceTexture, UTexture2D *targetTexture)
{
	// Do some error checking
	//if (sourceTexture->GetNumMips() != targetTexture->GetNumMips())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Cannot copy texture data because the number of mips doesn't match up!!"));
	//	return;
	//}

	//if (sourceTexture->GetSizeX() != targetTexture->GetSizeY() || sourceTexture->GetSizeY() != targetTexture->GetSizeY())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Cannot copy texture data because the textures are not the same size!!"));
	//	return;
	//}

	// How many mips we're going to update
	int32 mipCount = targetTexture->GetNumMips();
	//int32 mipCount = 1;

	// Get a reference to the pixel data we're going to copy
	TArray<uint8*> TexData;
	//= sourceTexture->Source.LockMip(0);

	// Lock all the mips for the source and target
	for (int mipIndex = 0; mipIndex < mipCount; mipIndex++)
	{
		TexData.Add(sourceTexture->PlatformData.Mips[mipIndex].BulkData.(LgmipIndex));
		targetTexture->Source.LockMip(mipIndex);
	}




	//const int32 TextureDataSize = InLeftEyeAtlasData.Num() * InLeftEyeAtlasData.GetTypeSize();
	//check(TextureDataSize == LeftEyeAtlas->GetSizeX() * LeftEyeAtlas->GetSizeY() * sizeof(FColor));

	//FTexture2DMipMap& Mip = LeftEyeAtlas->PlatformData->Mips[0];
	//void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	//FMemory::Memcpy(Data, InLeftEyeAtlasData.GetData(), TextureDataSize);
	//Mip.BulkData.Unlock();
	//LeftEyeAtlas->UpdateResource();



	// Disable streaming on the target while we copy over the data
	//targetTexture->TemporarilyDisableStreaming();

	// Copy over the data from the source to the target texture (with some temp hardcoded sizes)
	for (int mipIndex = 0; mipIndex < mipCount; mipIndex++)
	{
		FUpdateTextureRegion2D Region(0, 0, 0, 0, targetTexture->PlatformData->Mips[mipIndex].SizeX, targetTexture->PlatformData->Mips[mipIndex].SizeY);
		targetTexture->UpdateTextureRegions(mipIndex, 1, &Region, targetTexture->PlatformData->Mips[mipIndex].SizeX * 4, 16, TexData[mipIndex]);
	}

	// Unlock the source and target mips
	for (int mipIndex = 0; mipIndex < mipCount; mipIndex++)
	{
		sourceTexture->Source.UnlockMip(mipIndex);
		targetTexture->Source.UnlockMip(mipIndex);
	}

	//// Re register all actor components to let the world know the lighting conditions have changed?  Not sure if this is necessary, need to test
	//for (TActorIterator<AActor> ActorItr(GWorld); ActorItr; ++ActorItr)
	//{
	//	(*ActorItr)->ReregisterAllComponents();
	//}
}

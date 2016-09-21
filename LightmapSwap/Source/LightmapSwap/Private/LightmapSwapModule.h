#pragma once

#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "LightmapSwap"

class LightmapSwapModule : public IModuleInterface
{

private:

	TSharedPtr<FExtender> MainMenuExtender;

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void CreateNewMenu();
	static void OnCreateNewMenu(FMenuBarBuilder& InMenuBarBuilder);

	static void OnCreateNewMenu2(FMenuBarBuilder& InMenuBarBuilder);

	static void createThumbnailMaterialInstance(FMenuBuilder& InMenuBarBuilder);

	static void colorChangeMaterial(FMenuBuilder& InMenuBarBuilder);

	static void loadLevelLightmaps(FMenuBuilder& InMenuBarBuilder);

	UFUNCTION(BlueprintCallable, Category = "Lightmap Saving")
	static TArray<UTexture2D *> getLightMapTextures();
};
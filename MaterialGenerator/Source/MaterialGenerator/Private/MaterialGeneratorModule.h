#pragma once

#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "MaterialGenerator"

class MaterialGeneratorModule : public IModuleInterface
{

private:

	TSharedPtr<FExtender> MainMenuExtender;

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void CreateNewMenu();
	static void OnCreateNewMenu(FMenuBarBuilder& InMenuBarBuilder);

	static void createMaterial(FMenuBuilder& InMenuBarBuilder);
};
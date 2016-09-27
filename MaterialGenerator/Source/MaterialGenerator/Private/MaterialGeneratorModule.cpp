#include "MaterialGeneratorModule.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Editor/MaterialEditor/Public/IMaterialEditor.h"
#include "Toolkits/ToolkitManager.h"

#undef WITH_EDITORONLY_DATA
#define WITH_EDITORONLY_DATA 1

IMPLEMENT_MODULE(MaterialGeneratorModule, NewMenu );

void MaterialGeneratorModule::StartupModule()
{
	CreateNewMenu();
}

void MaterialGeneratorModule::ShutdownModule()
{

}

void MaterialGeneratorModule::CreateNewMenu()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>( "LevelEditor" );
	MainMenuExtender = MakeShareable( new FExtender );
	MainMenuExtender->AddMenuBarExtension( "Help",
										EExtensionHook::After,
										NULL,
										FMenuBarExtensionDelegate::CreateStatic( &MaterialGeneratorModule::OnCreateNewMenu ) );

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
}

void MaterialGeneratorModule::OnCreateNewMenu( FMenuBarBuilder& InMenuBarBuilder )
{
	InMenuBarBuilder.AddPullDownMenu
	(
		LOCTEXT( "MaterialCreateEditor1", "Create new Material" ),
		LOCTEXT( "MaterialCreateEditorTooltip1", "Generate a new Kilograph material" ),
		FNewMenuDelegate::CreateStatic(&MaterialGeneratorModule::createMaterial)
	);
}

void MaterialGeneratorModule::createMaterial(FMenuBuilder& InMenuBarBuilder)
{
	
}

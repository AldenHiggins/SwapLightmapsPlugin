#include "LightmapSwapModule.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Editor/MaterialEditor/Public/IMaterialEditor.h"
#include "Toolkits/ToolkitManager.h"

#undef WITH_EDITORONLY_DATA
#define WITH_EDITORONLY_DATA 1

IMPLEMENT_MODULE(LightmapSwapModule, NewMenu );

void LightmapSwapModule::StartupModule()
{
	CreateNewMenu();
}

void LightmapSwapModule::ShutdownModule()
{

}

void LightmapSwapModule::CreateNewMenu()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>( "LevelEditor" );
	MainMenuExtender = MakeShareable( new FExtender );
	MainMenuExtender->AddMenuBarExtension( "Help",
										EExtensionHook::After,
										NULL,
										FMenuBarExtensionDelegate::CreateStatic( &LightmapSwapModule::OnCreateNewMenu ) );
}

void LightmapSwapModule::OnCreateNewMenu( FMenuBarBuilder& InMenuBarBuilder )
{
	InMenuBarBuilder.AddPullDownMenu
	(
		LOCTEXT( "NewMenuEditor1", "Save Current Lightmap" ),
		LOCTEXT( "NewMenuEditorTooltip1", "Save out the current lightmap for this level" ),
		FNewMenuDelegate::CreateStatic(&LightmapSwapModule::loadLevelLightmaps)
	);
}

void LightmapSwapModule::OnCreateNewMenu2(FMenuBarBuilder& InMenuBarBuilder)
{
	InMenuBarBuilder.AddPullDownMenu
	(
		LOCTEXT("NewMenuEditor2", "Material Color Change"),
		LOCTEXT("NewMenuEditorTooltip2", "Prepare material to have it's color changed"),
		FNewMenuDelegate::CreateStatic(&LightmapSwapModule::colorChangeMaterial)
	);
}

void LightmapSwapModule::loadLevelLightmaps(FMenuBuilder& InMenuBarBuilder)
{
	//// Get the selected asset from the content browser
	//FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	//TArray<FAssetData> selectedAssets;
	//ContentBrowserModule.Get().GetSelectedAssets(selectedAssets);
	//// Exit out if the user hasn't selected anything
	//if (selectedAssets.Num() == 0)
	//{
	//	return;
	//}

	//// Get the selected material
	//UMapProperty* selected = (UMaterial *)selectedAssets[0].GetAsset();


	//TArray<UTexture2D*> LightMapsAndShadowMaps;
	//GWorld->GetLightMapsAndShadowMaps(World->GetCurrentLevel(), LightMapsAndShadowMaps);

	//for (auto ObjIt = LightMapsAndShadowMaps.CreateConstIterator(); ObjIt; ++ObjIt)
	//{
	//	UTexture2D* CurrentObject = *ObjIt;
	//	if (CurrentObject)
	//	{
	//		FAssetData AssetData = FAssetData(CurrentObject);
	//		const uint32 ThumbnailResolution = 64;
	//		TSharedPtr<FAssetThumbnail> LightMapThumbnail = MakeShareable(new FAssetThumbnail(AssetData, ThumbnailResolution, ThumbnailResolution, ThumbnailPool));
	//		TSharedPtr<FLightmapItem> NewItem = MakeShareable(new FLightmapItem(CurrentObject->GetPathName(), LightMapThumbnail));
	//		LightmapItems.Add(NewItem);
	//	}
	//}
}

TArray<UTexture2D *> LightmapSwapModule::getLightMapTextures()
{
	TArray<UTexture2D*> LightMapsAndShadowMaps;
	GWorld->GetLightMapsAndShadowMaps(GWorld->GetCurrentLevel(), LightMapsAndShadowMaps);

	return LightMapsAndShadowMaps;
}

void LightmapSwapModule::colorChangeMaterial(FMenuBuilder& InMenuBarBuilder)
{
	// Get the selected asset from the content browser
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> selectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(selectedAssets);
	// Exit out if the user hasn't selected anything
	if (selectedAssets.Num() == 0)
	{
		return;
	}

	// Get the selected material
	UMaterial* selected = (UMaterial *)selectedAssets[0].GetAsset();

	// Get the material expressions
	TArray<class UMaterialExpression*> *expressions = &selected->Expressions;
	// Exit out if no expressions can be found
	if (expressions->Num() == 0)
	{
		return;
	}

	FColorMaterialInput baseColor = selected->BaseColor;
	UMaterialExpression *colorExpression = baseColor.Expression;

	// Create the scalar parameters for the material
	UMaterialExpressionScalarParameter *uCoordinateScalar = NewObject<UMaterialExpressionScalarParameter>((UObject *)selected, UMaterialExpressionScalarParameter::StaticClass(), NAME_None, RF_Transactional);
	uCoordinateScalar->MaterialExpressionEditorX = 200;
	uCoordinateScalar->MaterialExpressionEditorY = 0;
	uCoordinateScalar->ParameterName = "ColorWheelU";
	selected->Expressions.Add(uCoordinateScalar);

	UMaterialExpressionScalarParameter *vCoordinateScalar = NewObject<UMaterialExpressionScalarParameter>((UObject *)selected, UMaterialExpressionScalarParameter::StaticClass(), NAME_None, RF_Transactional);
	vCoordinateScalar->MaterialExpressionEditorX = 200;
	vCoordinateScalar->MaterialExpressionEditorY = 200;
	vCoordinateScalar->ParameterName = "ColorWheelV";
	selected->Expressions.Add(vCoordinateScalar);

	UMaterialExpressionScalarParameter *colorBlendScalar = NewObject<UMaterialExpressionScalarParameter>((UObject *)selected, UMaterialExpressionScalarParameter::StaticClass(), NAME_None, RF_Transactional);
	colorBlendScalar->MaterialExpressionEditorX = 200;
	colorBlendScalar->MaterialExpressionEditorY = 400;
	colorBlendScalar->ParameterName = "ColorBlendAlpha";
	selected->Expressions.Add(colorBlendScalar);

	// Get the color change material function
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	UMaterialFunction *materialFunction = LoadObject<UMaterialFunction>(NULL, TEXT("/Game/KilographPlayer/Materials/Functions/ApplyColorChange.ApplyColorChange"), NULL, LOAD_None, NULL);
	// Create the color change expression
	UMaterialExpressionMaterialFunctionCall* newFunctionCall = NewObject<UMaterialExpressionMaterialFunctionCall>(selected);
	newFunctionCall->MaterialExpressionEditorX = 400;
	newFunctionCall->MaterialExpressionEditorY = 0;
	newFunctionCall->MaterialFunction = materialFunction;
	newFunctionCall->UpdateFromFunctionResource();
	expressions->Add(newFunctionCall);

	// Set the function inputs to the newly created parameters
	TArray<struct FFunctionExpressionInput> *functionInputs = &(newFunctionCall->FunctionInputs);
	vCoordinateScalar->ConnectExpression(&(*functionInputs)[0].Input, 0);
	uCoordinateScalar->ConnectExpression(&(*functionInputs)[3].Input, 0);
	colorExpression->ConnectExpression(&(*functionInputs)[2].Input, 0);
	colorBlendScalar->ConnectExpression(&(*functionInputs)[1].Input, 0);

	selected->BaseColor.Expression = newFunctionCall;

	// Compile the material
	selected->ForceRecompileForRendering();

	UE_LOG(LogTemp, Warning, TEXT("Expressisons found!!!"));
}

void LightmapSwapModule::createThumbnailMaterialInstance(FMenuBuilder& InMenuBarBuilder)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> selectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(selectedAssets);

	if (selectedAssets.Num() == 0)
	{
		return;
	}
	
	// Get the thumbnail material that we will create an instance of
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	UMaterial *TheMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/FirstPerson/Materials/FurnitureIcons/ThumbnailMaterial.ThumbnailMaterial"), NULL, LOAD_None, NULL);

	// Get the package name of the thumbnail material
	FString Name;
	FString PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(TheMaterial->GetOutermost()->GetName(), TEXT("_Mat"), PackageName, Name);
	
	// Add _Mat to the name of the thumbnail image
	UTexture* selected = (UTexture *) selectedAssets[0].GetAsset();
	FString selectedName = selectedAssets[0].AssetName.ToString();
	selectedName += "_Mat";

	// Create the new material instance
	UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
	Factory->InitialParent = CastChecked<UMaterialInterface>(TheMaterial);
	UObject* NewAsset = AssetToolsModule.Get().CreateAsset(selectedName, FPackageName::GetLongPackagePath(PackageName), UMaterialInstanceConstant::StaticClass(), Factory);
	// Set the texture parameter to be the selected texture
	UMaterialInstanceConstant *newlyCreatedMaterial = (UMaterialInstanceConstant *)NewAsset;
	newlyCreatedMaterial->SetTextureParameterValueEditorOnly("ThumbTexture", selected);
}




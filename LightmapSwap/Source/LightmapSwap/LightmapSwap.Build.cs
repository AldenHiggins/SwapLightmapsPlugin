// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LightmapSwap : ModuleRules
{
    public LightmapSwap(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(
            new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"UnrealEd",
				"Slate",
                "SlateCore",
                "ImageWrapper",
                "AssetTools",
                "ContentBrowser"
            }
		);
		//Private Paths
        PrivateIncludePaths.AddRange
        (
            new[]
            {
                "LightmapSwap/Private"
            }
        );

        PrivateIncludePathModuleNames.AddRange
        (
            new string[]
            {
                "AssetTools"
            }
        );
    }
}

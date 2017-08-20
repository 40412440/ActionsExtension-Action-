// Copyright 2015-2017 Piperift. All Rights Reserved.

using UnrealBuildTool;

public class AIExtension : ModuleRules
{
	public AIExtension(ReadOnlyTargetRules TargetRules) : base(TargetRules)
    {
		PublicIncludePaths.AddRange(new string[]
        {
            "AIExtension/Public"
			// ... add public include paths required here ...
	    });
				
		
		PrivateIncludePaths.AddRange(new string[]
        {
			"AIExtension/Private"
			// ... add other private include paths required here ...
		});
			
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
            "GameplayTasks",
            "AIModule"
			// ... add other public dependencies that you statically link with here ...
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "GameplayTags"
        });


        if (UEBuildConfiguration.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "SlateCore",
                    "Slate"
                }
            );
        }

        if (UEBuildConfiguration.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
        {
            PrivateDependencyModuleNames.Add("GameplayDebugger");
            Definitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
        }
        else
        {
            Definitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
        }
    }
}

// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE

using UnrealBuildTool;

public class OceanPlugin : ModuleRules
{
    public OceanPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.AddRange(new string[] { });

        PrivateIncludePaths.AddRange(new string[] { });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "Landscape"
		});


        PrivateDependencyModuleNames.AddRange(new string[] {            
            "CoreUObject",
            "Engine",
            "RHI",
            "RenderCore"
            // ... add private dependencies that you statically link with here ...	
        });

        DynamicallyLoadedModuleNames.AddRange(new string[] { });
    }
}
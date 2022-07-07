// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE

using UnrealBuildTool;
using System.IO;

public class OceanPlugin : ModuleRules
{
    public OceanPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		var SkyPluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../SkyPlugin/"));
		var WeatherDataPluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../WeatherDataPlugin/"));
		 
		bool bUsingSkyPlugin = Directory.Exists(SkyPluginDir);
		bool bUsingWeatherDataPlugin = Directory.Exists(WeatherDataPluginDir);
		
        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Landscape"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "RHI",
                "RenderCore"
                // ... add private dependencies that you statically link with here ...	
            }
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
            );
			if(bUsingSkyPlugin){
			
				PrivateDependencyModuleNames.AddRange(new string[]{"SkyPlugin",});
				PublicDependencyModuleNames.AddRange(new string[]{"SkyPlugin",});
				PublicIncludePaths.AddRange(new string[] {Path.Combine(SkyPluginDir, "Source/public/")});
				
				
				PublicDefinitions.Add("WITH_SKYPLUGIN=1");
			}else{PublicDefinitions.Add("WITH_SKYPLUGIN=0");}
			
			if(bUsingWeatherDataPlugin){
				PrivateDependencyModuleNames.AddRange(new string[]{"WeatherDataPlugin",});
				PublicDependencyModuleNames.AddRange(new string[]{"WeatherDataPlugin",});
				PublicIncludePaths.AddRange(new string[] {Path.Combine(WeatherDataPluginDir, "Source/public/")});
				
				
				PublicDefinitions.Add("WITH_WEATHERDATAPLUGIN=1");
			}else{PublicDefinitions.Add("WITH_WEATHERDATAPLUGIN=0");}

        // Make sure UBT reminds us of how to keep the project IWYU compliant
        bEnforceIWYU = true;
    }
}
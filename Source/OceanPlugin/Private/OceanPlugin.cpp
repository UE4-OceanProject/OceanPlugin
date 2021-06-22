// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE

#include "OceanPlugin.h"

#define LOCTEXT_NAMESPACE "FOceanPluginModule"

//Not needed if using FDefaultGameModuleImpl below
//void FOceanPluginModule::StartupModule()
//{
//	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//}
//
//void FOceanPluginModule::ShutdownModule()
//{
//	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
//	// we call this function before unloading the module.
//}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDefaultGameModuleImpl, OceanPlugin)

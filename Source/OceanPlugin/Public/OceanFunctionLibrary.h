// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE
#pragma once
#include "CoreMinimal.h"
#if WITH_SKYPLUGIN
#include"SkyManager.h"
#include "Misc/OutputDeviceNull.h"
#endif
#include "OceanFunctionLibrary.generated.h"


UCLASS()
class OCEANPLUGIN_API UOceanFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_UCLASS_BODY()
		//Will call UpdateSun function inside BP_sky if SkyPlugin was installed, otherwise will do nothing.
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static void UpdateSun(UObject* WorldContextObject);
};
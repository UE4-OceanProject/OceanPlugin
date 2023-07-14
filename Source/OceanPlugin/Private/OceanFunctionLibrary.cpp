#include "OceanFunctionLibrary.h"

UOceanFunctionLibrary::UOceanFunctionLibrary(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UOceanFunctionLibrary::UpdateSun(UObject* WorldContextObject) {

#if WITH_SKYPLUGIN
#if WITH_EDITOR
	if (GIsEditor)
	{
		FEditorScriptExecutionGuard ScriptGuard; // to make CallFunctionByNameWithArguments run in the editor 
	}
#endif //WITHEDITOR
	ASkyManager* SkyManager = nullptr;
	//May not be the best way as hardcoded text maynot be a good practice
	UClass* BPSkyClass = FindObject<UClass>(nullptr, TEXT("/SkyPlugin/BP_Sky.BP_Sky_C"));
	if (BPSkyClass) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject->GetWorld(), BPSkyClass, FoundActors);
		if (FoundActors.Num() > 0)
			SkyManager = Cast<ASkyManager>(FoundActors[0]);
	}
	if (SkyManager) {
		FOutputDeviceNull OutputDeviceNull;

		SkyManager->CallFunctionByNameWithArguments(TEXT("UpdateSun"), OutputDeviceNull, nullptr, true);
	}
#endif //WITH_SKYPLUGIN


}
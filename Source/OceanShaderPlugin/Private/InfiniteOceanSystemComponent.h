#pragma once

#include "CoreMinimal.h"
#include <Components/SceneComponent.h>
#include "InfiniteOceanSystemComponent.generated.h"


UENUM()
enum EFollowMethod2
{
	LookAtLocation2,
	FollowCamera2,
	FollowPawn2,
	Stationary2
};

/** 
 *	Infinite Ocean Plane System.
 *	Follows camera and scales by distance to make a plane appear as infinite.
 */
UCLASS(hidecategories=(Object, Mobility, LOD), ClassGroup=Physics, showcategories=Trigger, meta=(BlueprintSpawnableComponent))
class OCEANSHADERPLUGIN_API UInfiniteOceanSystemComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool UpdateInEditor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TEnumAsByte<enum EFollowMethod2> FollowMethod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float GridSnapSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MaxLookAtDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool ScaleByDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ScaleDistanceFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ScaleStartDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ScaleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ScaleMax;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;

protected:
	UWorld* World;
};

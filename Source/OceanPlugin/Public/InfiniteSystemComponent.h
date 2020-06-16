// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE

#pragma once

#include "CoreMinimal.h"
#include <Components/SceneComponent.h>
#include "InfiniteSystemComponent.generated.h"


UENUM(BlueprintType)
enum class EFollowMethod : uint8
{
	LookAtLocation UMETA(DisplayName = "Ocean moves to where we are looking"),
	FollowCamera UMETA(DisplayName = "Ocean moves to where the camera is"),
	FollowPawn UMETA(DisplayName = "Ocean moves to where the pawn is"),
	Stationary UMETA(DisplayName = "Ocean stays where its spawned")
};

/** 
 *	Infinite Ocean Plane System.
 *	Follows camera and scales by distance to make a plane appear as infinite.
 */
UCLASS(hidecategories=(Object, Mobility, LOD), ClassGroup=Physics, showcategories=Trigger, meta=(BlueprintSpawnableComponent))
class OCEANPLUGIN_API UInfiniteSystemComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UInfiniteSystemComponent(const class FObjectInitializer& ObjectInitializer);

	//Allows the infinite ocean system to update while in editor mode
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		bool UpdateInEditor = true;

	//Determines the follow behavior of the infinite system
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		EFollowMethod FollowMethod = EFollowMethod::FollowCamera;

	//Can be used to update the infinite system location only after x amount of distance
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float GridSnapSize = 0.0f;

	//When using the LookAt follow method, determines the maximum distance away from the camera the system will travel
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float MaxLookAtDistance = 10000.0;

	//Sets whether the infinite system will automatically scale basede on distance from the surface
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		bool ScaleByDistance = true;

	//Controls the level of scaling allowed when using scale by distance
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float ScaleDistanceFactor = 1000.0f;

	//Controls how far away the start of the scale distance is
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float ScaleStartDistance = 1.0f;

	//The minimum scaling of the infinite system
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float ScaleMin = 1.0f;

	//The maximum scaleing of the infinite system
	UPROPERTY(Category = "Infinite Ocean System", BlueprintReadWrite, EditAnywhere)
		float ScaleMax = 15.0f;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;

protected:
	UWorld* World;
};

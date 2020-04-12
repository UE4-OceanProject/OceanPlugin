#include "InfiniteOceanSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Engine/World.h>


UInfiniteOceanSystemComponent::UInfiniteOceanSystemComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bTickInEditor = true;
	
	//Defaults
	UpdateInEditor = true;
	GridSnapSize = 0;
	MaxLookAtDistance = 20000;
	ScaleByDistance = true;
	ScaleDistanceFactor = 1000;
	ScaleStartDistance = 1;
	ScaleMin = 1;
	ScaleMax = 100;

	World = GetWorld();
}

void UInfiniteOceanSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	//Store the world ref.
	World = GetWorld();
}

void UInfiniteOceanSystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If disabled or we are not attached to a parent component, return.
	if (!bIsActive || !GetAttachParent() || !World) return;

	FVector CamLoc;
	FRotator CamRot;
	FVector PawnLoc;
	FVector NewLoc;

#if WITH_EDITOR
	if (GIsEditor)
	{
		if (!UpdateInEditor) return;

		TArray<FVector> viewLocations = GetWorld()->ViewLocationsRenderedLastFrame;
		if (viewLocations.Num() != 0)
		{
			CamLoc = viewLocations[0];
		}

		//FEditorViewportClient* client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		//CamLoc = client->GetViewLocation();
		//CamRot = client->GetViewRotation();

		if (FollowMethod == LookAtLocation2 || FollowMethod == FollowCamera2)
		{
			NewLoc = CamLoc;
			NewLoc = NewLoc.GridSnap(GridSnapSize);
			NewLoc.Z = GetAttachParent()->GetComponentLocation().Z;
			GetAttachParent()->SetWorldLocation(NewLoc);
		}
		else
		{
			GetAttachParent()->SetRelativeLocation(FVector(0, 0, 0)); //Reset location
		}

		float Distance = FMath::Abs(CamLoc.Z - GetAttachParent()->GetComponentLocation().Z);

		if (ScaleByDistance && Distance > ScaleStartDistance)
		{
			Distance = Distance - ScaleStartDistance;
			Distance = FMath::Max(Distance, 0.f);

			float DistScale = Distance / ScaleDistanceFactor;
			DistScale = FMath::Clamp(DistScale, ScaleMin, ScaleMax);
			GetAttachParent()->SetRelativeScale3D(FVector(DistScale, DistScale, 1));
		}
		else if (ScaleByDistance)
		{
			GetAttachParent()->SetRelativeScale3D(FVector(ScaleMin, ScaleMin, 1));
		}
		else
		{
			GetAttachParent()->SetRelativeScale3D(FVector(1, 1, 1));
		}

// 		if (ScaleByDistance && FMath::Abs(CamLoc.Z - AttachParent->GetComponentLocation().Z) > ScaleStartDistance)
// 		{
// 			float DistScale = FMath::Abs(CamLoc.Z - AttachParent->GetComponentLocation().Z) / ScaleDistanceFactor;
// 			DistScale = FMath::Clamp(DistScale, ScaleMin, ScaleMax);
// 			AttachParent->SetRelativeScale3D(FVector(DistScale, DistScale, 1)); //Scale only on x & y axis
// 		}
// 		else if (!ScaleByDistance)
// 		{
// 			AttachParent->SetRelativeScale3D(FVector(1, 1, 1)); //Reset scale
// 		}
		return;
	}
#endif

	if (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE)
	{
		if (!UGameplayStatics::GetPlayerController(World, 0)) return;
		if (!UGameplayStatics::GetPlayerController(World, 0)->PlayerCameraManager) return;
		UGameplayStatics::GetPlayerController(World, 0)->PlayerCameraManager->GetCameraViewPoint(CamLoc, CamRot);

		if (UGameplayStatics::GetPlayerPawn(World, 0)) //null check
		{
			PawnLoc = UGameplayStatics::GetPlayerController(World, 0)->GetPawn()->GetActorLocation();
		}
		else
		{
			PawnLoc = GetAttachParent()->GetComponentLocation();
		}
	}

	switch (FollowMethod)
	{
		case LookAtLocation2:
			if (!FMath::SegmentPlaneIntersection(CamLoc, CamLoc + CamRot.Vector() * MaxLookAtDistance, FPlane(GetAttachParent()->GetComponentLocation(), FVector(0, 0, 1)), NewLoc))
			{
				NewLoc = CamLoc + CamRot.Vector() * MaxLookAtDistance;
			}
			break;
		case FollowCamera2:
			NewLoc = CamLoc;
			break;
		case FollowPawn2:
			NewLoc = PawnLoc;
			break;
		default:
			break;
	};

	//UE_LOG(LogTemp, Warning, TEXT("Camera Z Distance from Plane: %f"), FMath::Abs(CamLoc.Z - AttachParent->GetComponentLocation().Z));

	float Distance = FMath::Abs(CamLoc.Z - GetAttachParent()->GetComponentLocation().Z);

	if (ScaleByDistance && Distance > ScaleStartDistance)
	{
		Distance = Distance - ScaleStartDistance;
		Distance = FMath::Max(Distance, 0.f);

		float DistScale = Distance / ScaleDistanceFactor;
		DistScale = FMath::Clamp(DistScale, ScaleMin, ScaleMax);
		GetAttachParent()->SetRelativeScale3D(FVector(DistScale, DistScale, 1));
	}
	else if (ScaleByDistance)
	{
		GetAttachParent()->SetRelativeScale3D(FVector(ScaleMin, ScaleMin, 1));
	}
	else
	{
		GetAttachParent()->SetRelativeScale3D(FVector(1, 1, 1));
	}

	if (FollowMethod == Stationary2) return;

	NewLoc = NewLoc.GridSnap(GridSnapSize);
	NewLoc.Z = GetAttachParent()->GetComponentLocation().Z;
	GetAttachParent()->SetWorldLocation(NewLoc);
}

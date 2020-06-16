// For copyright see LICENSE in EnvironmentProject root dir, or:
//https://github.com/UE4-OceanProject/OceanProject/blob/Master-Environment-Project/LICENSE

#pragma once

#include "CoreMinimal.h"
#include "Landscape.h"
#include "Materials/Material.h"
#include "OceanManager.generated.h"

//Which OceanMaterial to use for rendering
UENUM(BlueprintType)
enum class EOceanMaterialType : uint8
{
	OceanOld UMETA(DisplayName = "Ocean Old"),
	OceanNoSSR UMETA(DisplayName = "Ocean (No SSR)"),
	OceanWSSR UMETA(DisplayName = "Ocean w/ SSR"),
	OceanWFoamNoSSR UMETA(DisplayName = "Ocean w/ Foam (No SSR)"),
	OceanWFoamAndCubemapNoSSR UMETA(DisplayName = "Ocean w/ Foam & Cubemap (No SSR)"),
	OceanWSSRAndFoam UMETA(DisplayName = "Ocean w/ SSR & Foam"),
	OceanWSSRAndFoamAndCubemap UMETA(DisplayName = "Ocean w/ SSR, Foam & Cubemap"),
	OceanWOGerstnerNoSSR UMETA(DisplayName = "Ocean w/o Gerstner (No SSR)"),
	OceanWOGerstnerWSSR UMETA(DisplayName = "Ocean w/o Gerstner w/ SSR"),
	OceanUltra UMETA(DisplayName = "Ocean Ultra"),
	OceanStorm UMETA(DisplayName = "Ocean Storm"),
	OceanUltraforTrueSky UMETA(DisplayName = "Ocean Ultra (for TrueSky)"),
};

/*
* Contains the parameters necessary for a single Gerstner wave.
*/
USTRUCT(BlueprintType)
struct OCEANPLUGIN_API FWaveParameter
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	float Rotation;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	float Length;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	float Amplitude;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	float Steepness;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	float TimeScale;

	FORCEINLINE FWaveParameter(float InRotation, float InLength, float InAmplitude, float InSteepness, float InTimeScale);

	//Default struct values
	FWaveParameter()
	{
		Rotation = 0.45f;
		Length = 1200.f;
		Amplitude = 100.f;
		Steepness = 0.8f;
		TimeScale = 1.f;
	}
};

FORCEINLINE FWaveParameter::FWaveParameter(float InRotation, float InLength, float InAmplitude, float InSteepness, float InTimeScale)
	: Rotation(InRotation), Length(InLength), Amplitude(InAmplitude), Steepness(InSteepness), TimeScale(InTimeScale)
{ }

/*
* Contains the parameters necessary for a set of Gerstner waves.
*/
USTRUCT(BlueprintType)
struct OCEANPLUGIN_API FWaveSetParameters
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave01;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave02;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave03;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave04;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave05;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave06;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave07;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
	FWaveParameter Wave08;

	//Default struct values
	FWaveSetParameters()
	{
		Wave01 = FWaveParameter(0.f, 1.05f, 1.4f, 1.2f, 1.f);
		Wave02 = FWaveParameter(-0.05f, 0.65f, 1.1f, 0.6f, 1.f);
		Wave03 = FWaveParameter(0.045f, 1.85f, 2.1f, 1.35f, 1.f);
		Wave04 = FWaveParameter(0.02f, 0.65f, 0.9f, 0.9f, 1.f);
		Wave05 = FWaveParameter(-0.015f, 1.28f, 1.854f, 1.2f, 1.f);
		Wave06 = FWaveParameter(0.065f, 0.75f, 1.15f, 0.5f, 1.f);
		Wave07 = FWaveParameter(0.01f, 1.15f, 1.55f, 1.15f, 1.f);
		Wave08 = FWaveParameter(-0.04f, 1.45f, 1.75f, 0.45f, 1.f);
	}
};


// Cache for the "dir" variable in CalculateGerstnerWaveHeight
struct FWaveCache
{
	bool GetDir(float rotation, const FVector2D& inDirection, FVector* outDir);
	void SetDir(float rotation, const FVector2D& inDirection, const FVector& inDir);

private:
	float LastRotation = 0.f;
	FVector2D LastDirection;
	FVector MemorizedDir;
};


/**
* OceanManager calculates the Gerstner waves in code, while the Material uses it's own implementation in a MaterialFunction.
* TODO: Investigate whether a single implementation could be used to increase performance.
*/
UCLASS(BlueprintType, Blueprintable)
class OCEANPLUGIN_API AOceanManager : public AActor
{
	GENERATED_BODY()

public:
	AOceanManager(const class FObjectInitializer& ObjectInitializer);

	//The darker shade of color to be blended on the ocean surface
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		FLinearColor BaseColorDark = FLinearColor(0.0f, 0.003f, 0.01f, 1.0f);

	//The lighter shade of color to be blended on the ocean surface
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		FLinearColor BaseColorLight = FLinearColor(0.00075f, 0.021857f, 0.055f, 1.0f);

	//The color that gets blended for shallow water
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		FLinearColor ShallowWaterColor = FLinearColor(0.145f, 0.22f, 0.26f, 1.0f);

	//Controls the blending factor for the surface color
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float BaseColorLerp = 0.85f;

	//The amount of fresnel to use in the depth fade
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float FresnelPower = 2.0f;

	//The amount of reflection for the fresnel
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float BaseFresnelReflect = 0.2f;

	//Controls the level of metallic on the ocean material
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float Metallic = 0.05f;

	//Controls the roughness value on the ocean material
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float Roughness = 0.015f;

	//Controls the specularity level for the material
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float Specular = 1.0f;

	//Controls the amount of tesselation
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float TesselationMultiplier = 1.0f;

	//Controls the overall opacity
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float Opacity = 1.0f;

	//While looking from above, this ontrols how deep you can see under water
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float BaseDepthFade = 600.0f;

	//Controls the distortion level
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float DistortionStrength = 0.03f;

	//Controls the amount of color you can see in very shallow water
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float SceneColorCustomDepth = 10000.0f;

	//Controls the size of the foam textures used at the shoreline
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float FoamScale = 3000.0f;

	//Controls how much shore foam you ca see for foam type 1
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float FoamDepth1 = 600.0f;

	//Controls how much shore foam you ca see for foam type 2
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float FoamDepth2 = 2000.0f;

	//Contols how quickly the shore foam textures pan
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		float FoamTimeScale = 1.0f;

	//Controls how hard the edges of the shore foam are
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "1"))
		float FoamSoftness = 0.1;

	//Controls how hard the color change from deep to shallow is
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "1"))
		float SceneDepthSoftness = 0.1f;

	//Controls how hard the color change from shallow to very shallow is
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "1"))
		float BaseDepthFadeSoftness = 0.9f;

	//Controls which ocean material is used for rendering
	UPROPERTY(Category = "Appearance", BlueprintReadWrite, EditAnywhere)
		EOceanMaterialType OceanShader = EOceanMaterialType::OceanUltra;




	//The color used for sub surface scattering at the crest of waves
	UPROPERTY(Category = "Subsurface Scattering", BlueprintReadWrite, EditAnywhere)
		FLinearColor SSS_Color = FLinearColor(0.3f, 0.7f, 0.6f, 0.0f);

	//Controls the scale of the sub surface scattering
	UPROPERTY(Category = "Subsurface Scattering", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "0.05", ClampMin = "0"))
		float SSS_Scale = 0.01;

	//Controls the color intensity of the sub surface scattering
	UPROPERTY(Category = "Subsurface Scattering", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "2", ClampMin = "0"))
		float SSS_Intensity = 0.4f;

	//Controls how deeply into a wave the sub surface scattering will penetrate
	UPROPERTY(Category = "Subsurface Scattering", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "1000", ClampMin = "0"))
		float SSS_LightDepth = 300.0;

	//Controls how much affect the sub surface scattering has on surface normals
	UPROPERTY(Category = "Subsurface Scattering", BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0", UIMax = "2", ClampMin = "0"))
		float SSS_MacroNormalStrength = 0.6f;


	//Controls how large the detail normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float DetailNormalScale = 20000.0f;

	//Controls the panning speed of the detail normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float DetailNormalSpeed = 0.05f;

	//Controls the strength of the detail normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float DetailNormalStrength = 0.0f;

	//Controls how large the medium normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float MediumNormalScale = 40000.0f;

	//Controls the panning speed of the medium normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float MediumNormalSpeed = 0.1f;

	//Controls the strength of the medium normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float MediumNormalStrength = 0.5f;

	//Controls how large the far away normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float FarNormalScale = 14000.0f;

	//Controls the panning speed of the far away normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float FarNormalSpeed = 1.0f;

	//Controls the strength of the far away normal texture
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float FarNormalStrength = 0.5f;

	//Controls how far in the distance blending of the far normals should occur
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float FarNormalBlendDistance = 300000.0f;

	//Controlls the amount of falloff for the far normal blending
	UPROPERTY(Category = "Detail, Medium & Far Normals", BlueprintReadWrite, EditAnywhere)
		float FarNormalBlendFalloff = 300000.0f;

	//OLD SHADER ONLY - Controls the blending between wave sizes
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float PanWaveLerp = 0.875f;

	//OLD SHADER ONLY - Controls the intensity of the smaller panning waves
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float PanWaveIntensity = 0.225f;

	//OLD SHADER ONLY - Controls thespeed of the smaller panning waves
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float PanWaveTimeScale = 0.85f;

	//OLD SHADER ONLY - Controls how large the small panning waves should be
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float PanWaveSize = 6800.0f;

	//OLD SHADER ONLY - Controls the first smaller wave speed
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		FVector Panner01Speed = FVector( -0.015, -0.05, 0.0 );

	//OLD SHADER ONLY - Controls the second smaller wave speed
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		FVector Panner02Speed = FVector( 0.02, -0.045, 0.0 );

	//OLD SHADER ONLY - Controls the third smaller wave speed
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		FVector Panner03Speed = FVector( -0.015, -0.085, 0.0 );

	//OLD SHADER ONLY - Controls the scale of the texture used by the tiny surface waves
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float MacroWaveScale = 1500.0f;

	//OLD SHADER ONLY - Controls the speed of the tiny surface waves
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float MacroWaveSpeed = 1.0f;

	//OLD SHADER ONLY - Controls the size of the tiny surface waves
	UPROPERTY(Category = "Panning Waves (Old Shader)", BlueprintReadWrite, EditAnywhere)
		float MacroWaveAmplify = 0.25f;



	//Controls the overall opacity of the wave caps
	UPROPERTY(Category = "Foam Wave Caps", BlueprintReadWrite, EditAnywhere)
		float FoamCapsOpacity = 0.8f;

	//Controls the minimum wave height the wave caps appear at
	UPROPERTY(Category = "Foam Wave Caps", BlueprintReadWrite, EditAnywhere)
		float FoamCapsHeight = 120.0f;

	//Controls the size of the foam caps
	UPROPERTY(Category = "Foam Wave Caps", BlueprintReadWrite, EditAnywhere)
		float FoamCapsPower = 4.0f;



	//Controls the scale of the offshore sea foam texture
	UPROPERTY(Category = "Seafoam (Based on Heightmap)", BlueprintReadWrite, EditAnywhere)
		float SeafoamScale = 3000.0f;

	//Controls the panning speed of the offshore sea foam
	UPROPERTY(Category = "Seafoam (Based on Heightmap)", BlueprintReadWrite, EditAnywhere)
		float SeafoamSpeed = 0.5f;

	//Controls the amount of distortion applied to the offshore sea foam texture
	UPROPERTY(Category = "Seafoam (Based on Heightmap)", BlueprintReadWrite, EditAnywhere)
		float SeafoamDistortion = 0.01f;

	//Controls the height offset for the offshore sea foam
	UPROPERTY(Category = "Seafoam (Based on Heightmap)", BlueprintReadWrite, EditAnywhere)
		float SeafoamHeightPower = 7.5f;

	//Controls how large a section of offshore sea foam will be
	UPROPERTY(Category = "Seafoam (Based on Heightmap)", BlueprintReadWrite, EditAnywhere)
		float SeafoamHeightMultiply = 2500.0;



	//Controls the scale of the heightmap based waves texture
	UPROPERTY(Category = "Heightmap", BlueprintReadWrite, EditAnywhere)
		float HeightmapScale = 1000.0f;

	//Controls the panning speed of the heightmap based waves
	UPROPERTY(Category = "Heightmap", BlueprintReadWrite, EditAnywhere)
		float HeightmapSpeed = 0.3f;

	//Controls the displacement of the heightmap based waves
	UPROPERTY(Category = "Heightmap", BlueprintReadWrite, EditAnywhere)
		float HeightmapDisplacement = 30.0f;





	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		bool EnableGerstnerWaves = true;

	// The global direction the waves travel.
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		FVector2D GlobalWaveDirection = FVector2D(0, 1);;

	// The global speed multiplier of the waves.
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float GlobalWaveSpeed = 2.0f;

	// The global amplitude multiplier of the waves.
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float GlobalWaveAmplitude = 1.0f;

	/* Optimization:
	* If the distance of a buoyant point to base sea level exceeds DistanceCheck,
	* skip the Gerstner calculations and return base sea level.
	*/
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float DistanceCheck = 10000.0f;

	/* Median Gerstner wave settings
	(only 1 cluster is used in the material by default).*/
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		TArray<FWaveParameter> WaveClusters;

	/* Individual Gerstner wave settings.
	(leave blank to use the default offsets).*/
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		TArray<FWaveSetParameters> WaveSetOffsetsOverride;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite)
		float NetWorkTimeOffset;

	UFUNCTION(BlueprintCallable, Category = "Ocean Manager", meta = (HidePin = "World"))
		FVector GetWaveHeightValue(const FVector& location, const UWorld* World = nullptr, bool HeightOnly = true, bool TwoIterations = false);

	// Returns the wave height at a determined location.
	// Same as GetWaveHeightValue, but only returns the vertical component.
	float GetWaveHeight(const FVector& location, const UWorld* World = nullptr) const;

	//Landscape height modulation vars.
	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		bool bEnableLandscapeModulation;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float ModulationStartHeight = -1000.0f;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float ModulationMaxHeight = 3200.0f;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		float ModulationPower = 4.0f;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		ALandscape* Landscape;

	UPROPERTY(Category = "Ocean", BlueprintReadWrite, EditAnywhere)
		UTexture2D* HeightmapTexture;

	UFUNCTION(BlueprintCallable, Category = "Ocean Manager")
		void LoadLandscapeHeightmap(UTexture2D* Tex2D);

	UFUNCTION(BlueprintCallable, Category = "Ocean Manager")
		FLinearColor GetHeightmapPixel(float U, float V) const;



	//CUBEMAP MATERIALS ONLY - Controls the strength of the cubemap reflection
	UPROPERTY(Category = "Cubemap Reflection", BlueprintReadWrite, EditAnywhere)
		float CubemapRelectionStrength = 0.3f;




	//The small wave normal map texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* SmallWaveNormal;

	//The medium wave normal map texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* MediumWaveNormal;

	//The far wave normal map texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* FarWaveNormal;

	//The primary shore foam diffuse texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* ShoreFoam;

	//The secondary shore foam diffuse texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* ShoreFoam2;

	//The roughness texture to apply to the shore foam
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* ShoreFoamRoughness;

	//These waves are used near the shoreline. 
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* HeightmapWaves;

	//The offshore foam diffuse texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* Seafoam;

	//CUBEMAP MATERIALS ONLY - The reflection cubemap texture
	UPROPERTY(Category = "Textures", BlueprintReadWrite, EditAnywhere)
		UTexture2D* ReflectionCubemap;

	//The ocean dynamic material instance
	UPROPERTY(Category = "Default", BlueprintReadWrite, EditAnywhere)
		UMaterialInstanceDynamic* MID_Ocean;

	//The depth plane dynamic material instance
	UPROPERTY(Category = "Default", BlueprintReadWrite, EditAnywhere)
		UMaterialInstanceDynamic* MID_Ocean_Depth;


private:

	virtual void BeginPlay() override;

	TArray<FFloat16Color> HeightmapPixels;
	int32 HeightmapWidth;
	int32 HeightmapHeight;

	mutable TArray<FWaveCache> WaveParameterCache;
	
	// Based on the parameters of the wave sets, the time and the position, computes the wave height.
	// Same as CalculateGerstnerWaveSetVector, but only returns the vertical component.
	float CalculateGerstnerWaveSetHeight(const FVector& position, float time) const;

	FVector CalculateGerstnerWaveSetVector(const FVector& position, float time, bool CalculateXY, bool CalculateZ) const;
	FVector CalculateGerstnerWaveVector(float rotation, float waveLength, float amplitude, float steepness, const FVector2D& direction, const FVector& position, float time, FWaveCache& InWaveCache, bool CalculateXY, bool CalculateZ) const;


	// Gets the time from the argument if it's not null, otherwise use GetWorld()
	float GetTimeSeconds(const UWorld* World) const;
};

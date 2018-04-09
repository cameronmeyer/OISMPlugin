#include "ParticleModuleColorBySpeed.h"
#include "CoreMinimal.h"
#include "UObject/UnrealType.h"
#include "UObject/Package.h"
#include "Particles/ParticleSystem.h"
#include "ParticleHelper.h"
#include "Distributions.h"
#include "Distributions/DistributionFloat.h"
#include "Particles/ParticleSystemComponent.h"
#include "Distributions/DistributionFloatConstant.h"
#include "Distributions/DistributionVectorConstant.h"
#include "Distributions/DistributionFloatParticleParameter.h"
#include "Distributions/DistributionVectorParticleParameter.h"
#include "Particles/Color/ParticleModuleColorBase.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Distributions/DistributionVectorConstantCurve.h"
#include "Distributions/DistributionFloatConstantCurve.h"
#include "Engine/InterpCurveEdSetup.h"

/*-----------------------------------------------------------------------------
UParticleModuleColorBySpeed implementation.
-----------------------------------------------------------------------------*/
UParticleModuleColorBySpeed::UParticleModuleColorBySpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
	bCurvesAsColor = true;
	bClampAlpha = true;
}

void UParticleModuleColorBySpeed::InitializeDefaults()
{
	if (!ColorBySpeed.IsCreated())
	{
		ColorBySpeed.Distribution = NewObject<UDistributionVectorConstantCurve>(this, TEXT("DistributionColorBySpeed"));
	}

	if (!AlphaBySpeed.IsCreated())
	{
		UDistributionFloatConstant* DistributionAlphaBySpeed = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionAlphaBySpeed"));
		DistributionAlphaBySpeed->Constant = 1.0f;
		AlphaBySpeed.Distribution = DistributionAlphaBySpeed;
	}
}

void UParticleModuleColorBySpeed::PostInitProperties()
{
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		ColorBySpeed.Distribution = NewObject<UDistributionVectorConstantCurve>(this, TEXT("DistributionColorBySpeed"));

		UDistributionFloatConstant* DistributionAlphaBySpeed = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionAlphaBySpeed"));
		DistributionAlphaBySpeed->Constant = 1.0f;
		AlphaBySpeed.Distribution = DistributionAlphaBySpeed;
	}
}

/*void UParticleModuleColorBySpeed::CompileModule(FParticleEmitterBuildInfo& EmitterInfo)
{
	bool ScaleColor = true;
	bool ScaleAlpha = true;
	if (IsUsedInGPUEmitter())
	{
		if (ColorBySpeed.Distribution->IsA(UDistributionVectorParticleParameter::StaticClass()))
		{
			EmitterInfo.DynamicColor = ColorBySpeed;
#if WITH_EDITOR
			EmitterInfo.DynamicColor.Distribution->bIsDirty = true;
			EmitterInfo.DynamicColor.Initialize();
#endif
			ScaleColor = false;
			EmitterInfo.ColorScale.InitializeWithConstant(FVector(1.0f, 1.0f, 1.0f));
		}

		if (AlphaBySpeed.Distribution->IsA(UDistributionFloatParticleParameter::StaticClass()))
		{
			EmitterInfo.DynamicAlpha = AlphaBySpeed;
#if WITH_EDITOR
			EmitterInfo.DynamicAlpha.Distribution->bIsDirty = true;
			EmitterInfo.DynamicAlpha.Initialize();
#endif
			ScaleAlpha = false;
			EmitterInfo.AlphaScale.InitializeWithConstant(1.0f);
		}
	}

	if (ScaleColor)
	{
		EmitterInfo.ColorScale.Initialize(ColorBySpeed.Distribution);
	}

	if (ScaleAlpha)
	{
		EmitterInfo.AlphaScale.Initialize(AlphaBySpeed.Distribution);
	}
}*/

#if WITH_EDITOR
void UParticleModuleColorBySpeed::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitializeDefaults();

	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged)
	{
		if (PropertyThatChanged->GetFName() == FName(TEXT("bClampAlpha")))
		{
			UObject* OuterObj = GetOuter();
			check(OuterObj);
			UParticleLODLevel* LODLevel = Cast<UParticleLODLevel>(OuterObj);
			if (LODLevel)
			{
				// The outer is incorrect - warn the user and handle it
				//UE_LOG(LogParticles, Warning, TEXT("UParticleModuleColorBySpeed has an incorrect outer... run FixupEmitters on package %s"),
					//*(OuterObj->GetOutermost()->GetPathName()));
				OuterObj = LODLevel->GetOuter();
				UParticleEmitter* Emitter = Cast<UParticleEmitter>(OuterObj);
				check(Emitter);
				OuterObj = Emitter->GetOuter();
			}
			UParticleSystem* PartSys = CastChecked<UParticleSystem>(OuterObj);

			//PartSys->UpdateColorModuleClampAlpha(this);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

bool UParticleModuleColorBySpeed::AddModuleCurvesToEditor(UInterpCurveEdSetup* EdSetup, TArray<const FCurveEdEntry*>& OutCurveEntries)
{
	bool bNewCurve = false;
#if WITH_EDITORONLY_DATA
	// Iterate over object and find any InterpCurveFloats or UDistributionFloats
	for (TFieldIterator<UStructProperty> It(GetClass()); It; ++It)
	{
		// attempt to get a distribution from a random struct property
		UObject* Distribution = FRawDistribution::TryGetDistributionObjectFromRawDistributionProperty(*It, (uint8*)this);
		if (Distribution)
		{
			FCurveEdEntry* Curve = NULL;
			if (Distribution->IsA(UDistributionFloat::StaticClass()))
			{
				// We are assuming that this is the alpha...
				if (bClampAlpha == true)
				{
					bNewCurve |= EdSetup->AddCurveToCurrentTab(Distribution, It->GetName(), ModuleEditorColor, &Curve, true, true, true, 0.0f, 1.0f);
				}
				else
				{
					bNewCurve |= EdSetup->AddCurveToCurrentTab(Distribution, It->GetName(), ModuleEditorColor, &Curve, true, true);
				}
			}
			else
			{
				// We are assuming that this is the color...
				bNewCurve |= EdSetup->AddCurveToCurrentTab(Distribution, It->GetName(), ModuleEditorColor, &Curve, true, true);
			}
			OutCurveEntries.Add(Curve);
		}
	}
#endif // WITH_EDITORONLY_DATA
	return bNewCurve;
}

void UParticleModuleColorBySpeed::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	SPAWN_INIT;
	float relativeSpeed = Particle.Velocity.Size();
	relativeSpeed = (relativeSpeed - MinSpeed) / (MaxSpeed - MinSpeed);
	FVector ColorVec = ColorBySpeed.GetValue(relativeSpeed, Owner->Component);
	float	fAlpha = AlphaBySpeed.GetValue(relativeSpeed, Owner->Component);
	Particle.Color.R = ColorVec.X;
	Particle.BaseColor.R = ColorVec.X;
	Particle.Color.G = ColorVec.Y;
	Particle.BaseColor.G = ColorVec.Y;
	Particle.Color.B = ColorVec.Z;
	Particle.BaseColor.B = ColorVec.Z;
	Particle.Color.A = fAlpha;
	Particle.BaseColor.A = fAlpha;
}

void UParticleModuleColorBySpeed::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	if ((Owner == NULL) || (Owner->ActiveParticles <= 0) ||
		(Owner->ParticleData == NULL) || (Owner->ParticleIndices == NULL))
	{
		return;
	}
	const FRawDistribution* FastColorBySpeed = ColorBySpeed.GetFastRawDistribution();
	const FRawDistribution* FastAlphaBySpeed = AlphaBySpeed.GetFastRawDistribution();
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride));
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
	if (FastColorBySpeed && FastAlphaBySpeed)
	{
		// fast path
		BEGIN_UPDATE_LOOP;
		{
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride));
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
			float relativeSpeed = Particle.Velocity.Size();
			relativeSpeed = (relativeSpeed - MinSpeed) / (MaxSpeed - MinSpeed);
			FastColorBySpeed->GetValue3None(relativeSpeed, &Particle.Color.R);
			FastAlphaBySpeed->GetValue1None(relativeSpeed, &Particle.Color.A);
		}
		END_UPDATE_LOOP;
	}
	else
	{
		FVector ColorVec;
		float	fAlpha;
		BEGIN_UPDATE_LOOP;
		{
			float relativeSpeed = Particle.Velocity.Size();
			relativeSpeed = (relativeSpeed - MinSpeed) / (MaxSpeed - MinSpeed);
			ColorVec = ColorBySpeed.GetValue(relativeSpeed, Owner->Component);
			fAlpha = AlphaBySpeed.GetValue(relativeSpeed, Owner->Component);
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride));
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
			Particle.Color.R = ColorVec.X;
			Particle.Color.G = ColorVec.Y;
			Particle.Color.B = ColorVec.Z;
			Particle.Color.A = fAlpha;
		}
		END_UPDATE_LOOP;
	}
}

void UParticleModuleColorBySpeed::SetToSensibleDefaults(UParticleEmitter* Owner)
{
	MinSpeed = 0.0f;
	MaxSpeed = 1.0f;
	ColorBySpeed.Distribution = NewObject<UDistributionVectorConstantCurve>(this);
	UDistributionVectorConstantCurve* ColorBySpeedDist = Cast<UDistributionVectorConstantCurve>(ColorBySpeed.Distribution);
	if (ColorBySpeedDist)
	{
		// Add two points, one at time 0.0f and one at 1.0f
		for (int32 Key = 0; Key < 2; Key++)
		{
			int32	KeyIndex = ColorBySpeedDist->CreateNewKey(Key * 1.0f);
			for (int32 SubIndex = 0; SubIndex < 3; SubIndex++)
			{
				if (Key == 0)
				{
					ColorBySpeedDist->SetKeyOut(SubIndex, KeyIndex, 1.0f);
				}
				else
				{
					ColorBySpeedDist->SetKeyOut(SubIndex, KeyIndex, 0.0f);
				}
			}
		}
		ColorBySpeedDist->bIsDirty = true;
	}

	AlphaBySpeed.Distribution = NewObject<UDistributionFloatConstantCurve>(this);
	UDistributionFloatConstantCurve* AlphaBySpeedDist = Cast<UDistributionFloatConstantCurve>(AlphaBySpeed.Distribution);
	if (AlphaBySpeedDist)
	{
		// Add two points, one at time 0.0f and one at 1.0f
		for (int32 Key = 0; Key < 2; Key++)
		{
			int32	KeyIndex = AlphaBySpeedDist->CreateNewKey(Key * 1.0f);
			if (Key == 0)
			{
				AlphaBySpeedDist->SetKeyOut(0, KeyIndex, 1.0f);
			}
			else
			{
				AlphaBySpeedDist->SetKeyOut(0, KeyIndex, 0.0f);
			}
		}
		AlphaBySpeedDist->bIsDirty = true;
	}
}
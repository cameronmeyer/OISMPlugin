// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Distributions/DistributionFloat.h"
#include "Distributions/DistributionVector.h"
#include "Particles/Color/ParticleModuleColorBase.h"
#include "ParticleModuleColorBySpeed.generated.h"

class UInterpCurveEdSetup;
class UParticleEmitter;
struct FCurveEdEntry;
struct FParticleEmitterInstance;

UCLASS(editinlinenew, hidecategories = Object, meta = (DisplayName = "Color By Speed"))
class UParticleModuleColorBySpeed : public UParticleModuleColorBase
{
	GENERATED_UCLASS_BODY()

		/** The color to apply to the particle, as a function of the particle RelativeTime. */
		UPROPERTY(EditAnywhere, Category = Color, meta = (TreatAsColor))
		struct FRawDistributionVector ColorBySpeed;

	/** The alpha to apply to the particle, as a function of the particle RelativeTime. */
	UPROPERTY(EditAnywhere, Category = Color)
		struct FRawDistributionFloat AlphaBySpeed;

	/** If true, the alpha value will be clamped to the [0..1] range. */
	UPROPERTY(EditAnywhere, Category = Color)
		uint32 bClampAlpha : 1;

	/** The minimum particle speed that corresponds to 0 on the color curve. */
	UPROPERTY(EditAnywhere)
		float MinSpeed;

	/** The maximum particle speed that corresponds to 1 on the color curve. */
	UPROPERTY(EditAnywhere)
		float MaxSpeed;

	/** Initializes the default values for this property */
	void InitializeDefaults();

	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	virtual void PostInitProperties() override;
	//~ End UObject Interface


	//Begin UParticleModule Interface
	virtual	bool AddModuleCurvesToEditor(UInterpCurveEdSetup* EdSetup, TArray<const FCurveEdEntry*>& OutCurveEntries) override;
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;
	//virtual void CompileModule(struct FParticleEmitterBuildInfo& EmitterInfo) override;
	virtual void SetToSensibleDefaults(UParticleEmitter* Owner) override;
	//End UParticleModule Interface

};




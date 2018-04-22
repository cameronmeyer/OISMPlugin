// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Distributions/DistributionFloat.h"
#include "ParticleModIntParticleModuleBase.h"
#include "ParticleModuleRotationBySpeed.generated.h"

struct FParticleEmitterInstance;

UCLASS(editinlinenew, hidecategories = Object, meta = (DisplayName = "Rotation By Speed"))
class UParticleModuleRotationBySpeed : public UParticleModIntParticleModuleBase
{
	GENERATED_UCLASS_BODY()

	/** The absolute rotation to apply to the particle, as a function of the particle MinSpeed and MaxSpeed.
	A rotation of 1.0 = 360 degrees. 
	A positive slope in the function rotates the particle clockwise.*/
	UPROPERTY(EditAnywhere)
	struct FRawDistributionFloat RotationBySpeed;

	/** The minimum particle speed that corresponds to 0 on the rotation curve. */
	UPROPERTY(EditAnywhere)
	float MinSpeed;

	/** The maximum particle speed that corresponds to 1 on the rotation curve. */
	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	/** Initializes the default values for this property */
	void InitializeDefaults();

	//Begin UObject Interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	virtual void	PostInitProperties() override;
	//End UObject Interface

	//Begin UParticleModule Interface
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;
	//End UParticleModule Interface

	/**
	*	Extended version of spawn, allows for using a random stream for distribution value retrieval
	*
	*	@param	Owner				The particle emitter instance that is spawning
	*	@param	Offset				The offset to the modules payload data
	*	@param	SpawnTime			The time of the spawn
	*	@param	InRandomStream		The random stream to use for retrieving random values
	*/
	void SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase);

	void RotateBySpeed(FBaseParticle& Particle, FParticleEmitterInstance* Owner);
};




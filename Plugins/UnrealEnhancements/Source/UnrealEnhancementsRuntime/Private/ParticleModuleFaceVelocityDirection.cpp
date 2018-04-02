#include "ParticleModuleFaceVelocityDirection.h"
#include "Distributions/DistributionFloatUniform.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystemComponent.h"

UParticleModuleFaceVelocityDirection::UParticleModuleFaceVelocityDirection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
	bInitialVelocityOnly = false;
}

void UParticleModuleFaceVelocityDirection::InitializeDefaults()
{
}

void UParticleModuleFaceVelocityDirection::PostInitProperties()
{
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		InitializeDefaults();
	}
}

#if WITH_EDITOR
void UParticleModuleFaceVelocityDirection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitializeDefaults();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UParticleModuleFaceVelocityDirection::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	SpawnEx(Owner, Offset, SpawnTime, NULL, ParticleBase);
}

void UParticleModuleFaceVelocityDirection::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	if (!bInitialVelocityOnly)
	{
		BEGIN_UPDATE_LOOP;
		{
			RotateByVelocity(Particle);
		}
		END_UPDATE_LOOP;
	}
}

void UParticleModuleFaceVelocityDirection::SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase)
{
	SPAWN_INIT;
	{
		RotateByVelocity(Particle);
		//Particle.Rotation += (PI/180.f) * 360.0f * StartRotation.GetValue(Owner->EmitterTime, Owner->Component, InRandomStream);
	}
}

void UParticleModuleFaceVelocityDirection::RotateByVelocity(FBaseParticle& Particle)
{
	FVector2D dir(Particle.Velocity.X, Particle.Velocity.Z);
	float angle = FMath::Atan2(dir.Y, dir.X);
	if (angle < 0.0f)
	{
		angle += PI * 2.0f;
	}

	Particle.Rotation = angle;
}

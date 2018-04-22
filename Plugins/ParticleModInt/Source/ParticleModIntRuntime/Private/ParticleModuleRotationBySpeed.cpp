#include "ParticleModuleRotationBySpeed.h"
#include "Distributions/DistributionFloatUniform.h"
#include "Distributions/DistributionFloatConstant.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystemComponent.h"

UParticleModuleRotationBySpeed::UParticleModuleRotationBySpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
}

void UParticleModuleRotationBySpeed::InitializeDefaults()
{
	if (!RotationBySpeed.IsCreated())
	{
		UDistributionFloatConstant* DistributionRotationBySpeed = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionRotationBySpeed"));
		DistributionRotationBySpeed->Constant = 0.0f;
		RotationBySpeed.Distribution = DistributionRotationBySpeed;
	}
}

void UParticleModuleRotationBySpeed::PostInitProperties()
{
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		InitializeDefaults();
	}
}

#if WITH_EDITOR
void UParticleModuleRotationBySpeed::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitializeDefaults();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UParticleModuleRotationBySpeed::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	SpawnEx(Owner, Offset, SpawnTime, NULL, ParticleBase);
}

void UParticleModuleRotationBySpeed::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	BEGIN_UPDATE_LOOP;
	{
		RotateBySpeed(Particle, Owner);
	}
	END_UPDATE_LOOP;
}

void UParticleModuleRotationBySpeed::SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase)
{
	SPAWN_INIT;
	{
		RotateBySpeed(Particle, Owner);
		//Particle.Rotation += (PI/180.f) * 360.0f * StartRotation.GetValue(Owner->EmitterTime, Owner->Component, InRandomStream);
	}
}

void UParticleModuleRotationBySpeed::RotateBySpeed(FBaseParticle& Particle, FParticleEmitterInstance* Owner)
{
	float relativeSpeed = Particle.Velocity.Size();
	relativeSpeed = (relativeSpeed - MinSpeed) / (MaxSpeed - MinSpeed);
	float angle = RotationBySpeed.GetValue(relativeSpeed, Owner->Component);
	angle = (PI / 180.0f) * 360.0f * angle;

	Particle.Rotation = angle;
}

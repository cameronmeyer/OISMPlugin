// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ParticleModIntRuntime.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/ConfigCacheIni.h"


static const FName ParticleModIntRuntimeTabName("ParticleModIntRuntime");

#define LOCTEXT_NAMESPACE "FParticleModIntRuntimeModule"

void FParticleModIntRuntimeModule::StartupModule()
{
	FString ConfigFile = FPaths::EnginePluginsDir() / TEXT("ParticleModInt/Content/ParticleModInt.ini");
	GConfig->LoadFile(ConfigFile);
}

void FParticleModIntRuntimeModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FParticleModIntRuntimeModule, ParticleModIntRuntime)
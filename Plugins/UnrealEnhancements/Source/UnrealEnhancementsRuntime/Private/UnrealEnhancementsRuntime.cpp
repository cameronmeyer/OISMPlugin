// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealEnhancementsRuntime.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/ConfigCacheIni.h"


static const FName UnrealEnhancementsRuntimeTabName("UnrealEnhancementsRuntime");

#define LOCTEXT_NAMESPACE "FUnrealEnhancementsRuntimeModule"

void FUnrealEnhancementsRuntimeModule::StartupModule()
{
	FString ConfigFile = FPaths::EnginePluginsDir() / TEXT("UnrealEnhancements/Content/UnrealEnhancements.ini");
	GConfig->LoadFile(ConfigFile);
}

void FUnrealEnhancementsRuntimeModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealEnhancementsRuntimeModule, UnrealEnhancementsRuntime)
// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ParticleModIntCommands.h"

#define LOCTEXT_NAMESPACE "FParticleModIntModule"

void FParticleModIntCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ParticleModInt", "Execute ParticleModInt action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

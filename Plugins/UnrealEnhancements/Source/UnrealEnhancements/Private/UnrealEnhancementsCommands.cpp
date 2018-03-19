// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealEnhancementsCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealEnhancementsModule"

void FUnrealEnhancementsCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UnrealEnhancements", "Execute UnrealEnhancements action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

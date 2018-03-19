// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UnrealEnhancementsStyle.h"

class FUnrealEnhancementsCommands : public TCommands<FUnrealEnhancementsCommands>
{
public:

	FUnrealEnhancementsCommands()
		: TCommands<FUnrealEnhancementsCommands>(TEXT("UnrealEnhancements"), NSLOCTEXT("Contexts", "UnrealEnhancements", "UnrealEnhancements Plugin"), NAME_None, FUnrealEnhancementsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};

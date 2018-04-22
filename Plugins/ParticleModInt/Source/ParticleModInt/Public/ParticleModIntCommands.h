// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ParticleModIntStyle.h"

class FParticleModIntCommands : public TCommands<FParticleModIntCommands>
{
public:

	FParticleModIntCommands()
		: TCommands<FParticleModIntCommands>(TEXT("ParticleModInt"), NSLOCTEXT("Contexts", "ParticleModInt", "ParticleModInt Plugin"), NAME_None, FParticleModIntStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};

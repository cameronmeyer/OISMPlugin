// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ParticleModInt.h"
#include "ParticleModIntStyle.h"
#include "ParticleModIntCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"


static const FName ParticleModIntTabName("ParticleModInt");

#define LOCTEXT_NAMESPACE "FParticleModIntModule"

void FParticleModIntModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FParticleModIntStyle::Initialize();
	FParticleModIntStyle::ReloadTextures();

	FParticleModIntCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	/*PluginCommands->MapAction(
		FParticleModIntCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FParticleModIntModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FParticleModIntModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FParticleModIntModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}*/

	// Find the display text for the tooltip with key ParticleModuleColorLife::ColorOverLife
	FTextDisplayStringPtr ptr = FTextLocalizationManager::Get().FindDisplayString(FString("UObjectToolTips"),
		FString("ParticleModuleColorOverLife:ColorOverLife"), nullptr);

	// Convert from shared ptr to shared ref
	FTextDisplayStringRef ref = ptr.ToSharedRef();

	// Update the display string reference to instead have the text "CHANGED"
	bool retVal = FTextLocalizationManager::Get().UpdateDisplayString(ref, FString("Sets a particle's color over its lifetime, a function of particle RelativeTime.\nMust contain the \"Particle Color\" node in the particle's material to change color."),
		FString("UObjectToolTips"), FString("ParticleModuleColorOverLife:ColorOverLife"));
}

void FParticleModIntModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FParticleModIntStyle::Shutdown();

	FParticleModIntCommands::Unregister();
}

void FParticleModIntModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FParticleModIntModule::PluginButtonClicked()")),
							FText::FromString(TEXT("ParticleModInt.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FParticleModIntModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FParticleModIntCommands::Get().PluginAction);
}

void FParticleModIntModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FParticleModIntCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FParticleModIntModule, ParticleModInt)
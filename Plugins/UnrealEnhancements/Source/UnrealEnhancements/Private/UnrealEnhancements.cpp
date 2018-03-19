// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealEnhancements.h"
#include "UnrealEnhancementsStyle.h"
#include "UnrealEnhancementsCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

static const FName UnrealEnhancementsTabName("UnrealEnhancements");

#define LOCTEXT_NAMESPACE "FUnrealEnhancementsModule"

void FUnrealEnhancementsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FUnrealEnhancementsStyle::Initialize();
	FUnrealEnhancementsStyle::ReloadTextures();

	FUnrealEnhancementsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealEnhancementsCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUnrealEnhancementsModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUnrealEnhancementsModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUnrealEnhancementsModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	// Find the display text for the tooltip with key ParticleModuleColorLife::ColorOverLife
	FTextDisplayStringPtr ptr = FTextLocalizationManager::Get().FindDisplayString(FString("UObjectToolTips"),
		FString("ParticleModuleColorOverLife:ColorOverLife"), nullptr);

	// Convert from shared ptr to shared ref
	FTextDisplayStringRef ref = ptr.ToSharedRef();

	// Update the display string reference to instead have the text "CHANGED"
	bool retVal = FTextLocalizationManager::Get().UpdateDisplayString(ref, FString("CHANGED"),
		FString("UObjectToolTips"), FString("ParticleModuleColorOverLife:ColorOverLife"));
}

void FUnrealEnhancementsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUnrealEnhancementsStyle::Shutdown();

	FUnrealEnhancementsCommands::Unregister();
}

void FUnrealEnhancementsModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FUnrealEnhancementsModule::PluginButtonClicked()")),
							FText::FromString(TEXT("UnrealEnhancements.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FUnrealEnhancementsModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUnrealEnhancementsCommands::Get().PluginAction);
}

void FUnrealEnhancementsModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUnrealEnhancementsCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealEnhancementsModule, UnrealEnhancements)
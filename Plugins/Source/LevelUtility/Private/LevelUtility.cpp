// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LevelUtility.h"
#include "LevelUtilityBPLibrary.h"
#include "Engine.h"
#include "HAL/IConsoleManager.h"

DEFINE_LOG_CATEGORY(LogLevelUtility);

void FLevelUtilityModule::StartupModule()
{
	static FAutoConsoleCommand CCmdSetLevelVisible(
		TEXT("LevelUtility.SetVisible"),
		TEXT("\n"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FLevelUtilityModule::CmdSetLevelVisible)
	);

	static FAutoConsoleCommand CCmdGetLevelVisible(
		TEXT("LevelUtility.GetVisible"),
		TEXT("\n"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FLevelUtilityModule::CmdGetLevelVisible)
	);

	static FAutoConsoleCommand CCmdSetLevelVisibleInEditor(
		TEXT("LevelUtility.SetVisibleLevelInEditor"),
		TEXT("\n"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FLevelUtilityModule::CmdSetLevelVisibleInEditor)
	);

	static FAutoConsoleCommand CCmdBuildLightingOnly(
		TEXT("LevelUtility.BuildLightingOnly"),
		TEXT("\n"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FLevelUtilityModule::CmdBuildLightingOnly)
	);
}

void FLevelUtilityModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


void FLevelUtilityModule::CmdSetLevelVisible(const TArray<FString>& Args)
{
	if (Args.Num() < 2)
	{
		UE_LOG(LogLevelUtility, Log, TEXT("LevelUtility.SetLevelVisible LevelName {true/false}"));
		return;
	}
	bool bVisilbe = (Args[1] == TEXT("true"));

	ULevelUtilityBPLibrary::SetLevelVisible(nullptr, FName(*Args[0]), bVisilbe);
}

void FLevelUtilityModule::CmdGetLevelVisible(const TArray<FString>& Args)
{
	if (Args.Num() < 1)
	{
		UE_LOG(LogLevelUtility, Log, TEXT("LevelUtility.GetLevelVisible LevelName"));
		return;
	}
	bool bVisible = ULevelUtilityBPLibrary::GetLevelVisible(nullptr, FName(*Args[0]));

	UE_LOG(LogLevelUtility, Log, TEXT("LevelUtility.GetLevelVisible Level:%s is %s"), *Args[0], bVisible ? TEXT("true") : TEXT("false"));
}

void FLevelUtilityModule::CmdSetLevelVisibleInEditor(const TArray<FString>& Args)
{
	if (Args.Num() < 2)
	{
		UE_LOG(LogLevelUtility, Log, TEXT("LevelUtility.SetLevelVisibleInEditor LevelName {true/false}"));
		return;
	}
	bool bVisilbe = (Args[1] == TEXT("true"));

	ULevelUtilityBPLibrary::SetLevelVisibleInEditor(FName(*Args[0]), bVisilbe);
}

void FLevelUtilityModule::CmdBuildLightingOnly(const TArray<FString>& Args)
{
	ULevelUtilityBPLibrary::BuildLightingOnly();
}

IMPLEMENT_MODULE(FLevelUtilityModule, LevelUtility)
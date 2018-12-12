// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLevelUtility, Log, All);


class FLevelUtilityModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void CmdSetLevelVisible(const TArray<FString>& Args);
	void CmdGetLevelVisible(const TArray<FString>& Args);
	void CmdSetLevelVisibleInEditor(const TArray<FString>& Args);
	void CmdBuildLightingOnly(const TArray<FString>& Args);
};

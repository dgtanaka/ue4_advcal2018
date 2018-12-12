// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LevelUtilityBPLibrary.h"
#include "LevelUtility.h"
#include "Engine.h"
#include "Engine/LevelStreaming.h"
#if WITH_EDITOR
#include "Editor.h"
#include "EditorLevelUtils.h"
#include "EditorBuildUtils.h"
#endif


ULevelUtilityBPLibrary::ULevelUtilityBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

ULevelStreaming* ULevelUtilityBPLibrary::FindLevel(const UObject* WorldContextObject, const FName levelName)
{
	UWorld* world = nullptr;

	if (!IsValid(WorldContextObject))
	{
		if (GEngine->GetWorldContexts().Num() == 0)
		{
			return nullptr;
		}
		world = GEngine->GetWorldContexts()[0].World();
	}
	else
	{
		world = WorldContextObject->GetWorld();
	}
	if (IsValid(world))
	{
		for (ULevelStreaming* level : world->GetStreamingLevels())
		{
#if WITH_EDITOR
			if (world->IsPlayInEditor())
			{
				if (world->RemovePIEPrefix(level->GetWorldAssetPackageFName().ToString()) == levelName.ToString())
				{
					return level;
				}
				continue;
			}
#endif
			if (level->GetWorldAssetPackageFName() == levelName)
			{
				return level;
			}
		}
	}
	return nullptr;
}


void ULevelUtilityBPLibrary::SetLevelVisible(const UObject* WorldContextObject, const FName levelName, bool bVisible)
{
	ULevelStreaming* level = ULevelUtilityBPLibrary::FindLevel(WorldContextObject, levelName);
	if (level)
	{
		level->SetShouldBeVisible(bVisible);
	}
	else
	{
		UE_LOG(LogLevelUtility, Warning, TEXT("GetLevelVisible: Unknown level:%s"), *levelName.ToString());
	}
}

bool ULevelUtilityBPLibrary::GetLevelVisible(const UObject* WorldContextObject, const FName levelName)
{
	ULevelStreaming* level = ULevelUtilityBPLibrary::FindLevel(WorldContextObject, levelName);
	if (level)
	{
		return level->GetShouldBeVisibleFlag();
	}
	else
	{
		UE_LOG(LogLevelUtility, Warning, TEXT("GetLevelVisible: Unknown level:%s"), *levelName.ToString());
	}
	return false;
}

TArray<FName> ULevelUtilityBPLibrary::GetLevelList(const UObject* WorldContextObject)
{
	TArray<FName>result;

	UWorld *world = WorldContextObject->GetWorld();
	if (!IsValid(world)) return result;

	for (ULevelStreaming* level : world->GetStreamingLevels())
	{
#if WITH_EDITOR
		if (world->IsPlayInEditor())
		{
			FName levelName = *world->RemovePIEPrefix(level->GetWorldAssetPackageFName().ToString());
			result.Add(levelName);
			continue;
		}
#else
		if (!level->GetWorldAssetPackageFName().IsNone())
		{
			result.Add(level->GetWorldAssetPackageFName());
		}
#endif
	}
	return result;

}

ULevelStreaming* ULevelUtilityBPLibrary::FindEditorLevel(const FName levelName)
{
#if WITH_EDITOR
	UWorld* world = nullptr;

	if (GEditor)
	{
		world = GEditor->GetEditorWorldContext().World();
		if (IsValid(world))
		{
			for (ULevelStreaming* level : world->GetStreamingLevels())
			{
				if (level->GetWorldAssetPackageFName() == levelName)
				{
					return level;
				}
			}
		}
	}
#endif
	return nullptr;
}

void ULevelUtilityBPLibrary::SetLevelVisibleInEditor(const FName levelName, bool bVisible)
{
#if WITH_EDITOR
	ULevelStreaming* level = ULevelUtilityBPLibrary::FindEditorLevel(levelName);
	if (level)
	{
		if (level->HasLoadedLevel())
		{
			ULevel* levelObject = level->GetLoadedLevel();
			EditorLevelUtils::SetLevelVisibility(levelObject, bVisible, false);

			levelObject->GetOutermost()->SetDirtyFlag(false);
		}
	}
	else
	{
		UE_LOG(LogLevelUtility, Warning, TEXT("GetLevelVisible: Unknown level:%s"), *levelName.ToString());
	}
#endif
}

void ULevelUtilityBPLibrary::BuildLightingOnly()
{
#if WITH_EDITOR
	UWorld* world = nullptr;

	if (GEngine)
	{
		world = GEditor->GetEditorWorldContext().World();
		if (IsValid(world))
		{
			FEditorBuildUtils::EditorBuild(world, FBuildOptions::BuildLighting, true);
		}
	}
#endif
}

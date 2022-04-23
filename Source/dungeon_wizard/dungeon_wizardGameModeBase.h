// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "DungeonGenerator.h"

#include "dungeon_wizardGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API Adungeon_wizardGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString&ErrorMessage) override;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AItem>> Resources;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADungeonGenerator> Generator;

protected:
	virtual void BeginPlay() override;
};

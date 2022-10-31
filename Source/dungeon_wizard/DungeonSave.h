// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DungeonSave.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API UDungeonSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	UDungeonSave();

	//UPROPERTY(BlueprintReadWrite)
		//FString GameName;
	UPROPERTY(BlueprintReadWrite)
		int CurrentRoom;
	UPROPERTY(BlueprintReadWrite)
		int RoomCount;

};

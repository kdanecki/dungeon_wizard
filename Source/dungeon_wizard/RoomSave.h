// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Room.h"
#include "NaturalResource.h"
#include "Faction.h"
#include "HumanBase.h"

#include "RoomSave.generated.h"


USTRUCT(BlueprintType)
struct FItemSave
{
	GENERATED_BODY()

	FItemSave()
	{
		CanBePickedUp = true;
	}

	FItemSave(AItem* Item)
	{
		ItemType = Item->GetClass();
		Transform = Item->GetActorTransform();
		CanBePickedUp = Item->CanBePickedUp;
	}

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<AItem> ItemType;
	UPROPERTY(BlueprintReadWrite)
		FTransform Transform;
	UPROPERTY(BlueprintReadWrite)
		bool CanBePickedUp;
};

USTRUCT(BlueprintType)
struct FCharacterSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<AHumanBase> CharacterType;
	UPROPERTY(BlueprintReadWrite)
		FTransform Transform;
	UPROPERTY(BlueprintReadWrite)
		float HP;
	UPROPERTY(BlueprintReadWrite)
		float Nourishment;
	UPROPERTY(BlueprintReadWrite)
		TArray<FItemSave> ItemsInfo;
};

USTRUCT(BlueprintType)
struct FFactionSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		TArray<FCharacterSave> MembersInfo;
};

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API URoomSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	URoomSave();

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<ARoom> RoomType;
	UPROPERTY(BlueprintReadWrite)
		FTransform Transform;
	UPROPERTY(BlueprintReadWrite)
		int BiomeIndex;
	UPROPERTY(BlueprintReadWrite)
		bool IsFinished;
	UPROPERTY(BlueprintReadWrite)
		TArray<int> NextRooms;
	UPROPERTY(BlueprintReadWrite)
		TArray<FResourceSave> ResourcesInfo;
	UPROPERTY(BlueprintReadWrite)
		TArray<FItemSave> ItemsInfo;
	UPROPERTY(BlueprintReadWrite)
		int FinishedDoor;
};

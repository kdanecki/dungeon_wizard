// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemPart.h"
#include "ToolBase.generated.h"

USTRUCT(BlueprintType)
struct FPartStats
{
	GENERATED_BODY()
	FPartStats()
	{
		
	}
	FPartStats(int Quality, int Durability, int UseSpeed)
		: Quality(Quality),
		  Durability(Durability),
		  UseSpeed(UseSpeed)
	{
	}

	UPROPERTY(BlueprintReadOnly)
	int Quality;
	UPROPERTY(BlueprintReadOnly)
	int Durability;
	UPROPERTY(BlueprintReadOnly)
	int UseSpeed;
};

UENUM(BlueprintType)
enum class EToolType : uint8 {
	PICKAXE,
	SHOVEL,
	SWORD
};

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AToolBase : public AItem
{
	GENERATED_BODY()

public:
	/*UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<AItemPart>> RequiredParts;
	virtual void Craft(const TArray<AItemPart*>& Parts);*/
	FPartStats Stats;
	Skladnik* Details;
	Skladnik* (*MixFunction)(Element*);
	
	
};

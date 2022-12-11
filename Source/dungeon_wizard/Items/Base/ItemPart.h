// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../../mixtures/skladnik.h"
#include "ItemPart.generated.h"

class AToolBase;

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

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AItemPart : public AItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AToolBase> PartOf;
	FPartStats Stats;
	Skladnik* Details;
	Skladnik* (*MixFunction)(Element*);
	
};

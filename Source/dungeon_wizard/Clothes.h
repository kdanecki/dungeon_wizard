// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Clothes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	NONE = 0,
	HEAD = 3,
	BODY = 4,
	LEGS = 5,
	FEET = 6,
	HANDS = 7,
	BLABLA = 8
};

UCLASS()
class DUNGEON_WIZARD_API AClothes : public AItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EBodyPart BodyPart;

};

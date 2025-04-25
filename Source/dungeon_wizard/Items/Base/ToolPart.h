// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../../mixtures/skladnik.h"
#include "PartStats.h"

#include "ToolPart.generated.h"

UENUM(BlueprintType)
enum class EToolType : uint8 {
	Point,
	Blade,
	Blunt
};

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AToolPart : public AItem
{
	GENERATED_BODY()

public:
	Skladnik* Detail;
	UPROPERTY(EditAnywhere)
	FPartStats Stats;
	EToolType Type;
};

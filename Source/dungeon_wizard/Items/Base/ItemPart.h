// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../../mixtures/skladnik.h"
#include "ItemPart.generated.h"

class AToolBase;



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

	
};

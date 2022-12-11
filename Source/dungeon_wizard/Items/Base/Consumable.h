// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceBase.h"
#include "Consumable.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AConsumable : public AResourceBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		float Nutrition;

};

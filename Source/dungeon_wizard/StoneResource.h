// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaturalResource.h"
#include "StoneResource.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AStoneResource : public ANaturalResource
{
	GENERATED_BODY()
	
public:
	virtual void Gather(ATool* Tool, FVector Location) override;
};

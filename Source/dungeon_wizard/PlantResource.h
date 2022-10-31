// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaturalResource.h"
#include "PlantResource.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API APlantResource : public ANaturalResource
{
	GENERATED_BODY()

public:
	virtual AResourceBase* HandGather() override;
	UPROPERTY(EditAnywhere)
		int MaxResources;

	UFUNCTION(BlueprintCallable)
		void Grow();
};

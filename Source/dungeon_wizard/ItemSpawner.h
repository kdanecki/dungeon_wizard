// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Item.h"

#include "ItemSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_WIZARD_API UItemSpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
		TMap<TSubclassOf<AActor>, float> PossibleItems;
};

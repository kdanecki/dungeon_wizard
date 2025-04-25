// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICharacterController.h"

#include "Faction.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AFaction : public AActor
{
	GENERATED_BODY()
public:
	AFaction();
protected:
	virtual void BeginPlay() override;

public:
	

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<AAICharacterController*> Members;
	UClass* cos;
	UFUNCTION(BlueprintCallable)
		void ActorSpotted(AActor* Actor);
	UFUNCTION(BlueprintImplementableEvent)
		FVector FindMigrateLocation();
	UFUNCTION()
		void Migrate();
};

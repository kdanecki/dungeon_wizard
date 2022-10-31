// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

class AFaction;

UENUM(BlueprintType)
enum class EAIAction : uint8
{
	WANDER UMETA(DisplayName = "Wander"),
	FIGHT UMETA(DisplayName = "Fight"),
	HUNT UMETA(DisplayName = "Hunt"),
	MIGRATE UMETA(DisplayName = "Migrate")
};
/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AAICharacterController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		AFaction* Faction;
	UPROPERTY(BlueprintReadOnly)
		EAIAction Action;
	UFUNCTION(BlueprintCallable)
		void SetAction(EAIAction NewAction, AActor* Actor, FVector Location);
};

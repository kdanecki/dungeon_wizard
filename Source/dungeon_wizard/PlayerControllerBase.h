// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanBase.h"

#include "PlayerControllerBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAction : uint8
{
	NONE = 0,
	CRAFT
};

UCLASS()
class DUNGEON_WIZARD_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerBase();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;
	virtual void OnPossess(APawn* InPawn) override;


public:
	UPROPERTY(Replicated)
	AHumanBase* PlayerPawn;
	virtual void SetupInputComponent() override;
	
	void Action();
	void SecondaryAction();

	UPROPERTY(BlueprintReadOnly, Replicated)
		EAction ActionMode;
	UPROPERTY(BlueprintReadOnly)
	TArray<AItem*> SelectedItems;

	UPROPERTY(BlueprintReadWrite)
		int CurrentRoom;

	UFUNCTION(Server, Reliable)
		void SetActionMode(EAction Mode);
	UFUNCTION(Client, Reliable)
	void SwitchModes();

	UFUNCTION(Server, Reliable)
		void CheckOutcomes(const TArray<AItem*>& Items);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Outcomes)
	TArray<UClass*> Outcomes;

	UFUNCTION(BlueprintImplementableEvent)
		void OnRep_Outcomes();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Craft(int Index, const TArray<AItem*>& Items);
};

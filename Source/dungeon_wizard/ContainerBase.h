// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clothes.h"
#include "ContainerBase.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AContainerBase : public AClothes
{
	GENERATED_BODY()
	
public:

	AContainerBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
		AContainerBase* ParentContainer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxCapacity;
	UPROPERTY(BlueprintReadOnly, Replicated)
		float Capacity;
	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<AItem*> Items;
	UFUNCTION()
		void AddItemToInventory(AItem* Item);
	UFUNCTION()
		void RemoveItemFromInventory(AItem* Item);
	UFUNCTION(BlueprintPure)
		bool CanContain(AItem* Item);
	UFUNCTION(BlueprintPure)
		bool CheckIfContains(AItem* Item);
	UFUNCTION(BlueprintPure)
		bool CheckParentContainer(AItem* Item);
	UFUNCTION(BlueprintPure)
		bool CheckForInventoryLoops(AContainerBase* Container, AContainerBase* MyContainer);
};

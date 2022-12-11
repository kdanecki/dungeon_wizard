// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemPart.h"
#include "ToolBase.generated.h"



UENUM(BlueprintType)
enum class EToolType : uint8 {
	PICKAXE,
	SHOVEL,
	SWORD
};

/**
 * 
 */
UCLASS()
class DUNGEON_WIZARD_API AToolBase : public AItem
{
	GENERATED_BODY()

public:
	/*UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<AItemPart>> RequiredParts;
	virtual void Craft(const TArray<AItemPart*>& Parts);*/
	Skladnik* Details;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AItemPart>> RequiredParts;
	virtual void Craft(const TArray<AItemPart*>& Parts);
	
};

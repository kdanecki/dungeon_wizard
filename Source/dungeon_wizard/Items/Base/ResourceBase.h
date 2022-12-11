// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../../mixtures/element.h"

#include "ResourceBase.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AResourceBase : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	Element* Detail;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Quantity;
	UPROPERTY(EditAnywhere)
		bool IsUnique;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float WeightPerQuantity;
	//void SetQuantity(float Number);
	//UPROPERTY(BlueprintReadWrite)
		//float Weight;
	UPROPERTY(BlueprintReadWrite)
		FString ItemType;
};

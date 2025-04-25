// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ToolHandle.h"
#include "ToolPart.h"

#include "Tool.generated.h"

UCLASS()
class DUNGEON_WIZARD_API ATool : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Craft(AToolHandle* PartHandle, AToolPart* Part);

	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	EToolType ToolType;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;
	UPROPERTY(EditAnywhere)
	float Durability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EaseOfUse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Precision;
	
	AToolHandle *Handle;
	AToolPart *Head;
	
};

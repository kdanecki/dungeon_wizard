// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolBase.h"
#include "Tool.generated.h"

UCLASS()
class DUNGEON_WIZARD_API ATool : public AToolBase
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

	UPROPERTY(EditAnywhere)
		float Damage;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere)
		float Range;
	UPROPERTY(EditAnywhere)
		EToolType ToolType;
};
